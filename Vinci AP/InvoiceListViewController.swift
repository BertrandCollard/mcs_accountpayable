//
//  CustomerListViewController.swift
//  swift demo mcs
//
//  Created by Bertrand Collard on 01/03/2016.
//  Copyright © 2016 Bertrand Collard. All rights reserved.
//

import UIKit

class InvoiceListViewController: UITableViewController {
    
    var deleteAPIndexPath: NSIndexPath? = nil
    
    var aps:[APObject] = []
    
    var logoutButton: UIBarButtonItem? = nil
    
    var addButton: UIBarButtonItem? = nil
    
    var conflictAP: APObject? = nil
    
    var forceAPsSyncPolicy: OMCSyncPolicy? = nil
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        //self.tableView.editing = true
        
        forceAPsSyncPolicy = OMCSyncPolicy()
        forceAPsSyncPolicy!.conflictResolution_policy = CONFLICT_RESOLUTION_POLICY_CLIENT_WINS
        forceAPsSyncPolicy!.fetch_Policy = FETCH_POLICY_FETCH_FROM_SERVICE
        forceAPsSyncPolicy!.expiration_Policy = EXPIRATION_POLICY_EXPIRE_AFTER
        forceAPsSyncPolicy!.eviction_Policy = EVICTION_POLICY_MANUAL_EVICTION
        forceAPsSyncPolicy!.update_Policy = UPDATE_POLICY_UPDATE_IF_ONLINE
        
        logoutButton = UIBarButtonItem(title: "Logout", style: .Plain, target: self, action: "logoutTapped:")
        addButton = UIBarButtonItem(title: "Add", style: .Plain, target: self, action: "addTapped:")
        self.navigationItem.rightBarButtonItem = self.editButtonItem()
        self.navigationItem.title = "Account Payable List"
        self.navigationItem.leftBarButtonItem = logoutButton
        
        self.refreshControl?.addTarget(self, action: "handleRefresh:", forControlEvents: UIControlEvents.ValueChanged)
        let tap = UISwipeGestureRecognizer(target: self, action: "handleSwipe:")
        //tap.direction = [UISwipeGestureRecognizerDirection.Down, UISwipeGestureRecognizerDirection.Up]
        self.tableView.addGestureRecognizer(tap)
        
        // Do any additional setup after loading the view.
        get_aps_from_MCS(nil)
    }
    
    func handleSwipe(gesture: UISwipeGestureRecognizer) {
        // handle tap
        print("handle Swipe")
        if gesture.state == UIGestureRecognizerState.Ended {
            if (gesture.direction == UISwipeGestureRecognizerDirection.Down) {
                print("Swipe Down")
            } else if (gesture.direction == UISwipeGestureRecognizerDirection.Right) {
                print("Swipe Right")
                switchOfflineOnline()
            }
        }
    }
    
    func handleRefresh(refreshControl: UIRefreshControl) {
        // Do some reloading of data and update the table view's data source
        // Fetch more objects from a web service, for example...
        
        // Simply adding an object to the data source for this example
        get_aps_from_MCS(refreshControl)
        //self.tableView.reloadData()
    }
    
    func get_aps_from_MCS(refreshControl: UIRefreshControl?){
        if let sync: OMCSynchronization = MyGlobalVar.omcMobileBackEnd!.synchronization(){
            
            MyGlobalVar.omcSynchronization = sync
            
            if (!MyGlobalVar.isSynchInitialized){
                //sync.initialize()
                sync.initializeWithMobileObjectEntities([APObject.self])
                MyGlobalVar.isSynchInitialized = true
            }
            
            NSRunLoop.currentRunLoop().runUntilDate(NSDate(timeIntervalSinceNow: 5))
            
            sync.cachedResourceChanged({ (uri: String!, resource: AnyObject!) -> Void in
                print("sync.cachedResourceChanged:" + uri)
                if (resource is OMCMobileObjectCollection) {
                    print ("OMCMobileObjectCollection")
                } else if (resource is APObject) {
                    let res: APObject = resource as! APObject
                    var result: NSString = ""
                    if (res.hasConflicts && self.conflictAP == nil){
                        self.conflictAP = res
                        result = "Conflict"
                        let reference = String(res.invoiceId) + " " + res.numberAp
                        print ("AP reference:"+reference)
                        self.PutCustomer_Conflit(reference)
                    } else if (res.hasOfflineUpdates) {
                        result = "Updated offline"
                    } else if (res.hasOfflineCommitError){
                        result = "Error commit offline"
                    } else {
                        result = "Success update"
                    }
                    print (result)
                }
            })
            
            MyGlobalVar.omcCustomerEndpoint = sync.openEndpoint(APObject.self, apiName: "invoicemanagment", endpointPath: "invoices")
            
            
            let builder: OMCFetchObjectCollectionBuilder = MyGlobalVar.omcCustomerEndpoint!.fetchObjectCollectionBuilder()
            //builder.setSyncPolicy(syncPolicy)
            
            print("Before call builder.executeFetchOnSuccess")
            
            builder.executeFetchOnSuccess({ (data) -> Void in
                data.pinResource(SyncPinPriority.High)
                MyGlobalVar.listCustomers = data
                self.reloadAP(data, refreshControl: refreshControl)
                }, onError: { (error: NSError!) -> Void in
                    self.GetCustomer_Error(error)
                }
            )
            
            print("After call builder.executeFetchOnSuccess")
        }
    }
    
    
    func reloadAP(data: OMCMobileResource, refreshControl: UIRefreshControl?) -> Void {
        data.reloadResource { (values: AnyObject!) -> Void in
            self.parseAPs(values.getMobileObjects())
            if (refreshControl != nil) {
                refreshControl!.endRefreshing()
            }
        }
    }
    
    func parseAPs(list: NSArray) -> Void {
        print("Enter in parseCustomers")
        
        aps.removeAll()
        for (var i = 0; i < list.count ; i++ ){
            let c: APObject = list[i] as! APObject
            aps.append(c)
        }
        do_table_refresh();
    }
    
    func GetAPs_Success(customersJSON: NSData) -> Void {
        extract_json(customersJSON)
    }
    
    func PutCustomer_Conflit(customer: String) -> Void {
        let alert = UIAlertController(title: "Conflicts", message: "Do you want to Force or Cancel update for \(customer)?", preferredStyle: .ActionSheet)
        
        let goForceAction = UIAlertAction(title: "Force", style: UIAlertActionStyle.Default, handler: handleForceUpdate)
        let goCancelAction = UIAlertAction(title: "Cancel", style: UIAlertActionStyle.Cancel, handler: handleCancelUpdate)
        
        alert.addAction(goForceAction)
        alert.addAction(goCancelAction)
        
        // Support display in iPad
        alert.popoverPresentationController?.sourceView = self.view
        alert.popoverPresentationController?.sourceRect = CGRectMake(self.view.bounds.size.width / 2.0, self.view.bounds.size.height / 2.0, 1.0, 1.0)
        
        self.presentViewController(alert, animated: true, completion: nil)
    }
    
    func handleForceUpdate(alertAction: UIAlertAction!) -> Void {
        let currentSyncPolicy = self.conflictAP?.getCurrentSyncPolicy()
        self.conflictAP?.setSyncPolicy(forceAPsSyncPolicy)
        self.conflictAP?.saveResourceOnSuccess({ (result : AnyObject!) -> Void in
                print("OK Force update to :" + String(self.conflictAP?.invoiceId))
                self.conflictAP?.setSyncPolicy(currentSyncPolicy)
                self.conflictAP = nil
                self.get_aps_from_MCS(nil)
            }, onError: { (error : NSError!) -> Void in
                print("KO Force update to :" + String(self.conflictAP?.invoiceId))
                print (error)
                self.conflictAP?.setSyncPolicy(currentSyncPolicy)
                self.conflictAP = nil
                self.get_aps_from_MCS(nil)
        })
    }
    
    func handleCancelUpdate(alertAction: UIAlertAction!) -> Void {
        self.conflictAP?.reloadResource(true, reloadFromService: true, onSuccess: { (result:AnyObject!) -> Void in
            self.get_aps_from_MCS(nil)
        })
        self.conflictAP = nil
    }
    
    func GetCustomer_Error(error: NSError) -> Void {
        print("Enter in GetCustomer_Error")
        let alertView:UIAlertView = UIAlertView()
        alertView.title = "Error calling executeFetchOnSuccess!"
        alertView.message = error.description
        alertView.delegate = self
        alertView.addButtonWithTitle("OK")
        alertView.show()
    }
    
    func GetCustomer_Msg(msg: String) -> Void {
        let alertView:UIAlertView = UIAlertView()
        alertView.title = "Error calling requestWithUri!"
        alertView.message = msg
        alertView.delegate = self
        alertView.addButtonWithTitle("OK")
        alertView.show()
    }
    
    
    
    override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return aps.count
    }
    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        
        let cell = tableView.dequeueReusableCellWithIdentifier("cell", forIndexPath: indexPath) as! InvoiceTableViewCell
        let ap = aps[indexPath.row]
        let formatter = NSDateFormatter()
        formatter.dateFormat = "dd/MM/yyyy"
        print (ap.totalAmount)
        cell.supplierNameField.text = ap.supplier
        cell.dateField.text = formatter.stringFromDate(ap.getDateApRef())
        cell.totalAmountField.text = String(ap.totalAmount)
        cell.poNumberField.text = ap.numberPo
        cell.statusField.text = ap.convertStatusToString()
        if (ap.status == 0){
            cell.statusField.textColor = UIColor.blueColor()
        } else if (ap.status == 1){
            cell.statusField.textColor = UIColor.greenColor()
        } else if (ap.status == 2){
            cell.statusField.textColor = UIColor.redColor()
        }
        return cell
    }
    
    func dateToString(date:NSDate) -> String {
        let dateFormatter = NSDateFormatter()
        dateFormatter.dateFormat = "hh:mm" //format style. Browse online to get a format that fits your needs.
        let dateString = dateFormatter.stringFromDate(date)
        return dateString
    }
    
    override func tableView(tableView: UITableView, canEditRowAtIndexPath indexPath: NSIndexPath) -> Bool {
        return true
    }
    
    override func tableView(tableView: UITableView, editActionsForRowAtIndexPath indexPath: NSIndexPath) -> [UITableViewRowAction]? {
        
        let delete = UITableViewRowAction(style: .Destructive, title: "Delete") { action, index in
            self.deleteAPIndexPath = indexPath
            let apToDelete = self.aps[indexPath.row]
            self.confirmDelete(apToDelete.supplier)
        }
        return [delete]
    }
    
    override func tableView(tableView: UITableView, commitEditingStyle editingStyle: UITableViewCellEditingStyle, forRowAtIndexPath indexPath: NSIndexPath) {
        print ("commitEditingStyle")
        if (editingStyle == UITableViewCellEditingStyle.Delete) {
            // handle delete (by removing the data from your array and updating the tableview)
            self.deleteAPIndexPath = indexPath
            let apToDelete = aps[indexPath.row]
            self.confirmDelete(apToDelete.supplier)
        }
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    
    /*
    // MARK: - Navigation
    
    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
    // Get the new view controller using segue.destinationViewController.
    // Pass the selected object to the new view controller.
    }
    */
    func logoutTapped(sender: UIBarButtonItem) {
        if (MyGlobalVar.omcAuthorization != nil){
            MyGlobalVar.omcAuthorization?.logout()
        }
        let appDomain = NSBundle.mainBundle().bundleIdentifier
        NSUserDefaults.standardUserDefaults().removePersistentDomainForName(appDomain!)
        let prefs:NSUserDefaults = NSUserDefaults.standardUserDefaults()
        prefs.removeObjectForKey("ISLOGGEDIN")
        prefs.synchronize()
        
        self.performSegueWithIdentifier("goto_login1", sender: self)
    }
    
    func addTapped(sender: UIBarButtonItem) {
        
        performSegueWithIdentifier("AddOrEditCustomer", sender: addButton)
    }
    
    func do_table_refresh()
    {
        dispatch_async(dispatch_get_main_queue(), {
            self.tableView.reloadData()
            return
        })
    }
    
    func switchOfflineOnline() {
        let alert = UIAlertController(title: "Online/Offline", message: "Do you want to switch?", preferredStyle: .ActionSheet)
        
        var statusOnlineAction = UIAlertActionStyle.Default
        var statusOfflineAction = UIAlertActionStyle.Default
        let status = MyGlobalVar.omcSynchronization?.getNetworkStatus()
        
        if (status == SyncNetworkStatus.Offline) {
            statusOnlineAction = UIAlertActionStyle.Cancel
        } else if (status == SyncNetworkStatus.Online) {
            statusOfflineAction = UIAlertActionStyle.Cancel
        } else if (status == SyncNetworkStatus.OfflineTest) {
            statusOnlineAction = UIAlertActionStyle.Cancel
        }
        
        let goOnlineAction = UIAlertAction(title: "Online", style: statusOnlineAction, handler: handleGoOnline)
        let goOfflineAction = UIAlertAction(title: "Offline", style: statusOfflineAction, handler: handleGoOffline)
        
        alert.addAction(goOnlineAction)
        alert.addAction(goOfflineAction)
        
        // Support display in iPad
        alert.popoverPresentationController?.sourceView = self.view
        alert.popoverPresentationController?.sourceRect = CGRectMake(self.view.bounds.size.width / 2.0, self.view.bounds.size.height / 2.0, 1.0, 1.0)
        
        self.presentViewController(alert, animated: true, completion: nil)
    }
    
    func handleGoOnline(alertAction: UIAlertAction!) -> Void {
        if let sync: OMCSynchronization = MyGlobalVar.omcMobileBackEnd!.synchronization(){
            sync.setOfflineMode(false)
        }
    }
    
    func handleGoOffline(alertAction: UIAlertAction!) -> Void {
        if let sync: OMCSynchronization = MyGlobalVar.omcMobileBackEnd!.synchronization(){
            sync.setOfflineMode(true)
        }
    }
    
    func confirmDelete(customer: String) {
        let alert = UIAlertController(title: "Delete Customer", message: "Are you sure you want to permanently delete \(customer)?", preferredStyle: .ActionSheet)
        
        let DeleteAction = UIAlertAction(title: "Delete", style: .Destructive, handler: handleDeleteCustomer)
        let CancelAction = UIAlertAction(title: "Cancel", style: .Cancel, handler: cancelDeleteCustomer)
        
        alert.addAction(DeleteAction)
        alert.addAction(CancelAction)
        
        // Support display in iPad
        alert.popoverPresentationController?.sourceView = self.view
        alert.popoverPresentationController?.sourceRect = CGRectMake(self.view.bounds.size.width / 2.0, self.view.bounds.size.height / 2.0, 1.0, 1.0)
        
        self.presentViewController(alert, animated: true, completion: nil)
    }
    
    func handleDeleteCustomer(alertAction: UIAlertAction!) -> Void {
        if let indexPath = deleteAPIndexPath {
            let ap = aps[indexPath.row]
            let apId = ap.invoiceId.stringValue
            tableView.beginUpdates()
            
            aps.removeAtIndex(indexPath.row)
            
            // Note that indexPath is wrapped in an array:  [indexPath]
            tableView.deleteRowsAtIndexPaths([indexPath], withRowAnimation: .Automatic)
            
            ap.deleteResourceOnError({ (error: NSError!) -> Void in
                print(error)
            })
            
            deleteAPIndexPath = nil
            
            tableView.endUpdates()
            
            let propertiesEvent: NSMutableDictionary = NSMutableDictionary()
            propertiesEvent.setValue(apId, forKey: "id")
            
            if (MyGlobalVar.omcAnalytics != nil) {
                MyGlobalVar.omcAnalytics?.logEvent("RemoveAP", properties: propertiesEvent as [NSObject : AnyObject])
                MyGlobalVar.omcAnalytics?.flush()
            }
        }
    }
    
    func cancelDeleteCustomer(alertAction: UIAlertAction!) {
        deleteAPIndexPath = nil
    }
    
    
    override func setEditing(editing: Bool, animated: Bool) {
        super.setEditing(editing, animated: animated)
        /*
        if (editing) {
            self.navigationItem.leftBarButtonItem = addButton
        } else {
            self.navigationItem.leftBarButtonItem = logoutButton
        }
        */
        //tableView.reloadData()
    }
    
    
    // storyboard segue
    
    override func prepareForSegue(segue: UIStoryboardSegue,
        sender: AnyObject!) {
            // sender is the tapped `UITableViewCell`
            if (sender is UITableViewCell) {
                let cell = sender as! UITableViewCell
                let indexPath = self.tableView.indexPathForCell(cell)
                
                // load the selected model
                let item = self.aps[indexPath!.row]
                
                let detail = segue.destinationViewController as! InvoiceViewController
                // set the model to be viewed
                
                detail.item = item
                detail.parent = self
                detail.updateType = true
            }
            if (sender is UIBarButtonItem) {
                let senderButton = sender as! UIBarButtonItem
                if (senderButton == addButton){
                    let detail = segue.destinationViewController as! InvoiceViewController
                    // set the model to be viewed
                    detail.item = nil
                    detail.updateType = false
                    detail.parent = self
                }
            }
    }
    
    func extract_json(jsonData:NSData)
    {
        print(NSString(data: jsonData, encoding: NSUTF8StringEncoding))
        do {
            let json: AnyObject? = try NSJSONSerialization.JSONObjectWithData(jsonData, options: .AllowFragments)
            if let customers_list = json as? NSArray
            {
                for (var i = 0; i < customers_list.count ; i++ )
                {
                    if let customer_obj = customers_list[i] as? NSDictionary
                    {
                        if let id = customer_obj["custid"] as? Int
                        {
                            if let firstname = customer_obj["firstname"] as? String
                            {
                                if let lastname = customer_obj["lastname"] as? String
                                {
                                    if let company = customer_obj["company"] as? String
                                    {
                                        if let jobtitle = customer_obj["jobtitle"] as? String
                                        {
                                            //aps.append(APObject(obj: id,  firstname: firstname, lastname: lastname, company: company, jobtitle: jobtitle))
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } catch {
            print("error serializing JSON: \(error)")
            //customers = customers_sample
        }
        do_table_refresh();
    }
    
}
