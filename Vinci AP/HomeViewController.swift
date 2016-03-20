//
//  HomeViewController.swift
//  swift demo mcs
//
//  Created by Bertrand Collard on 15/02/2016.
//  Copyright © 2016 Bertrand Collard. All rights reserved.
//

import UIKit

class HomeViewController: UIViewController {
    
    @IBOutlet weak var usernameLabel: UILabel!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Do any additional setup after loading the view.
        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        appDelegate.initializeNotificationServices()
        
    }
    @IBAction func logoutTapped(sender: UIButton) {
        if (MyGlobalVar.omcAuthorization != nil){
            MyGlobalVar.omcAuthorization?.logout()
        }
        let appDomain = NSBundle.mainBundle().bundleIdentifier
        NSUserDefaults.standardUserDefaults().removePersistentDomainForName(appDomain!)
        let prefs:NSUserDefaults = NSUserDefaults.standardUserDefaults()
        prefs.removeObjectForKey("ISLOGGEDIN")
        prefs.synchronize()
        
        self.performSegueWithIdentifier("goto_login", sender: self)
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
    
    override func viewDidAppear(animated: Bool) {
        super.viewDidAppear(true)
        
        let prefs:NSUserDefaults = NSUserDefaults.standardUserDefaults()
        let isLoggedIn:Int = prefs.integerForKey("ISLOGGEDIN") as Int
        if (isLoggedIn != 1) {
            self.performSegueWithIdentifier("goto_login", sender: self)
        } else {
            self.usernameLabel.text = prefs.valueForKey("USERNAME") as? String
            let username: String = (prefs.valueForKey("USERNAME") as? String)!
            let password: String = (prefs.valueForKey("PASSWORD") as? String)!
            self.initMCS(username, password: password)
            self.performSegueWithIdentifier("goto_customerview", sender: self)
        }
    }
    
    func initMCS(username: String, password: String) -> Void {
        let bem = OMCMobileBackendManager.sharedManager()
        MyGlobalVar.omcMobileBackEnd = bem.defaultMobileBackend
        if (MyGlobalVar.omcMobileBackEnd) != nil
        {
            let auth : OMCAuthorization! = MyGlobalVar.omcMobileBackEnd!.authorization
            if (auth != nil) {
                
                MyGlobalVar.omcAuthorization = auth
                if let err : NSError = auth.authenticate(username, password: password) {
                    NSLog("Authentication error: %@", err.localizedDescription);
                    let alertView:UIAlertView = UIAlertView()
                    alertView.title = "Sign in Failed!"
                    alertView.message = err.localizedDescription
                    alertView.delegate = self
                    alertView.addButtonWithTitle("OK")
                    alertView.show()
                    return ;
                }
            }
            if let sync: OMCSynchronization = MyGlobalVar.omcMobileBackEnd!.synchronization(){
                MyGlobalVar.omcSynchronization = sync
            }
            if let analytics : OMCAnalytics = MyGlobalVar.omcMobileBackEnd!.analytics() {
                MyGlobalVar.omcAnalytics = analytics
            }
            if let notifications : OMCNotifications = MyGlobalVar.omcMobileBackEnd!.notifications() {
                MyGlobalVar.omcNotifications = notifications
            }
        }
    }
    
}
