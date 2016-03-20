		//
//  CustomerViewController.swift
//  swift demo mcs
//
//  Created by Bertrand Collard on 02/03/2016.
//  Copyright © 2016 Bertrand Collard. All rights reserved.
//

import UIKit

class InvoiceViewController: UIViewController, UIScrollViewDelegate {
    
    var parent: InvoiceListViewController?
    var item: APObject?
    var updateType: Bool = true
    var updateButton: UIBarButtonItem? = nil
    
    @IBOutlet weak var idField: UITextField!
    
    @IBOutlet weak var apNumberField: UITextField!
    
    @IBOutlet weak var apDateField: UITextField!
    
    @IBOutlet weak var supplierField: UITextField!
    
    @IBOutlet weak var siretField: UITextField!

    @IBOutlet weak var poNumberField: UITextField!
    
    @IBOutlet weak var poDateField: UITextField!
    
    @IBOutlet weak var tvaField: UITextField!
    
    @IBOutlet weak var totalField: UITextField!
    
    @IBOutlet weak var statusField: UITextField!
    
    @IBOutlet weak var apImageViewField: UIImageView!
    
    @IBOutlet weak var imageScrollView: UIScrollView!
    
    @IBOutlet weak var acceptButton: UIButton!
    
    @IBOutlet weak var rejectButton: UIButton!
    
    @IBAction func acceptAction(sender: AnyObject) {
        item!.status = 1
        //item!.addOrUpdateJsonProperty("status", propertyValue: "1")
        updateItem()
    }
    
    @IBAction func rejectAction(sender: AnyObject) {
        item!.addOrUpdateJsonProperty("status", propertyValue: "2")
        updateItem()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        let formatter = NSDateFormatter()
        formatter.dateFormat = "dd/MM/yyyy"
        //self.tableView.editing = true
        
        if (updateType){
            updateButton = UIBarButtonItem(title: "Done", style: .Plain, target: self, action: "doneTapped:")
            idField.text = String(item!.invoiceId)
            apNumberField.text = item!.numberAp
            apDateField.text = formatter.stringFromDate(item!.getDateApRef())
            supplierField.text = item!.supplier
            siretField.text = String(item!.siret)
            poNumberField.text = item!.numberPo
            poDateField.text = formatter.stringFromDate(item!.getDatePoRef())
            tvaField.text = item!.tvaCode
            totalField.text = String(item!.totalAmount)
            statusField.text = item!.convertStatusToString()
            if (item!.base64Image != nil) {
                let imageData = NSData(base64EncodedString: item!.base64Image, options: .IgnoreUnknownCharacters)
                let image:UIImage = UIImage(data: imageData!)!
                apImageViewField.image = image
                apImageViewField.userInteractionEnabled = true
                apImageViewField.multipleTouchEnabled = true
                apImageViewField.clipsToBounds = true
                //apImageViewField.contentMode = UIViewContentMode.ScaleAspectFit
                apImageViewField.contentMode = UIViewContentMode.Center;
                if (apImageViewField.bounds.size.width > image.size.width && apImageViewField.bounds.size.height > image.size.height) {
                    apImageViewField.contentMode = UIViewContentMode.ScaleAspectFit;
                }
            }
            if (item!.status == 0){
                statusField.textColor = UIColor.blueColor()
                acceptButton.enabled = true
                rejectButton.enabled = true
            } else if (item!.status == 1){
                statusField.textColor = UIColor.greenColor()
                acceptButton.enabled = false
                rejectButton.enabled = false
            } else if (item!.status == 2){
                statusField.textColor = UIColor.redColor()
                acceptButton.enabled = false
                rejectButton.enabled = false
            }
            self.navigationItem.title = "Edit"
        } else {
            updateButton = UIBarButtonItem(title: "Submit", style: .Plain, target: self, action: "submitTapped:")
            //updateButton?.enabled = Reachability.isConnectedToNetwork()
            self.navigationItem.title = "Add"
        }
        
        self.navigationItem.rightBarButtonItem = updateButton
        
        //self.navigationItem.leftBarButtonItem = logoutButton
        // Do any additional setup after loading the view.
        
        self.imageScrollView.minimumZoomScale = 1.0;
        self.imageScrollView.maximumZoomScale = 1.0;
        self.imageScrollView.delegate = self
        self.imageScrollView.scrollEnabled = true
        
    }
    
    func viewForZoomingInScrollView(scrollView: UIScrollView) -> UIView?
    {
        return self.apImageViewField
    }
    
    func updateItem() -> Void {
        print (self.item!.status)
        item!.saveResource(true,onSuccess: { (newInvoice: AnyObject!) -> Void in
            
            self.item? = newInvoice as! APObject
            print (self.item!.status)
            
            if (self.item!.hasConflicts) {
                self.OMC_Synchronization_Error(String(self.item!.invoiceId))
                self.item!.reloadResource({ (result: AnyObject!) -> Void in
                    print(result)
                    self.updateAnalytic(self.idField.text!)
                })
            } else {
                self.updateAnalytic(self.idField.text!)
            }
            }) { (error: NSError!) -> Void in
                print (error)
        }
        self.parent?.get_aps_from_MCS(nil)
        navigationController!.popViewControllerAnimated(true)
    }
    
    func doneTapped(sender: UIBarButtonItem) {
        navigationController!.popViewControllerAnimated(true)
    }
    
    func updateAnalytic(id: NSString) -> Void {
        let propertiesEvent: NSMutableDictionary = NSMutableDictionary()
        propertiesEvent.setValue(id, forKey: "custid")
        
        if (MyGlobalVar.omcAnalytics != nil) {
            MyGlobalVar.omcAnalytics?.logEvent("UpdateAP", properties: propertiesEvent as [NSObject : AnyObject])
            MyGlobalVar.omcAnalytics?.flush()
        }
    }
    
    func OMC_Synchronization_Error(id: String) -> Void {
        let alertView:UIAlertView = UIAlertView()
        alertView.title = "Synchronization!"
        alertView.message = "Conflict on: " + id
        alertView.delegate = self
        alertView.addButtonWithTitle("OK")
        alertView.show()
    }
    
}
