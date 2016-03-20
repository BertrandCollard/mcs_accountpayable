//
//  LoginViewController.swift
//  swift demo mcs
//
//  Created by Bertrand Collard on 15/02/2016.
//  Copyright © 2016 Bertrand Collard. All rights reserved.
//

import UIKit

struct MyGlobalVar {
    static var omcNotifications: OMCNotifications? = nil
    static var omcAuthorization: OMCAuthorization? = nil
    static var omcDeviceToken: NSData? = nil
    static var omcMobileBackEnd: OMCMobileBackend? = nil
    static var omcAnalytics: OMCAnalytics? = nil
    static var omcSynchronization: OMCSynchronization? = nil
    static var isSynchInitialized: Bool = false
    static var omcCustomerEndpoint: OMCMobileEndpoint? = nil
    static var listCustomers: OMCMobileObjectCollection? = nil
}

class LoginViewController: UIViewController {

    @IBOutlet weak var userNameField: UITextField!
    
    @IBOutlet weak var passwordField: UITextField!
    
    func getOMCAuthentification() -> OMCAuthorization? {
        // Global variable omcNotifications
        
        if MyGlobalVar.omcAuthorization != nil {
            return MyGlobalVar.omcAuthorization!
        }
        return nil
    }
    
    func getOMCNotifications(username:String, password:String) -> OMCNotifications? {
        

        let bem = OMCMobileBackendManager.sharedManager()
        
        //if let be : OMCMobileBackend = bem.addMobileBackendForName("swift_lvm_bc", properties : props as[NSObject : AnyObject])
        MyGlobalVar.omcMobileBackEnd = bem.defaultMobileBackend
        if (MyGlobalVar.omcMobileBackEnd) != nil
        {
            let auth : OMCAuthorization! = MyGlobalVar.omcMobileBackEnd!.authorization
            auth.offlineAuthenticationEnabled = true
            NSLog("User : %@ / %@",username,password);
            if let err : NSError = auth.authenticate(username, password: password) {
                NSLog("Authentication error: %@", err.localizedDescription);
                let alertView:UIAlertView = UIAlertView()
                alertView.title = "Sign in Failed!"
                alertView.message = err.localizedDescription
                alertView.delegate = self
                alertView.addButtonWithTitle("OK")
                alertView.show()
                return nil;
            }
            NSLog("Authenticated as user: %@", auth.userName)
            MyGlobalVar.omcAuthorization = auth
            
            if let analytics : OMCAnalytics = MyGlobalVar.omcMobileBackEnd!.analytics() {
                MyGlobalVar.omcAnalytics = analytics
            }
            
            if let notifications : OMCNotifications = MyGlobalVar.omcMobileBackEnd!.notifications() {
                NSLog("Create notifications object OK!")
                return notifications
            } else {
                NSLog("Cannot create notifications object!")
                let alertView:UIAlertView = UIAlertView()
                alertView.title = "Sign in Failed!"
                alertView.message = "Cannot create notifications object!"
                alertView.delegate = self
                alertView.addButtonWithTitle("OK")
                alertView.show()
                return nil
            }
        }
        return nil
    }
    
    @IBAction func signInTapped(sender: UIButton) {
        
        // Authentication code
        let username:NSString = userNameField.text!
        let password:NSString = passwordField.text!
        
        if ( username.isEqualToString("") || password.isEqualToString("") ) {
            
            let alertView:UIAlertView = UIAlertView()
            alertView.title = "Sign in Failed!"
            alertView.message = "Please enter Username and Password"
            alertView.delegate = self
            alertView.addButtonWithTitle("OK")
            alertView.show()
        } else {
            MyGlobalVar.omcNotifications = getOMCNotifications(username as String, password: password as String)
            
            if (MyGlobalVar.omcNotifications != nil && MyGlobalVar.omcDeviceToken != nil ){
                convertDeviceTokenToString(MyGlobalVar.omcDeviceToken!)
                MyGlobalVar.omcNotifications!.registerForNotifications(MyGlobalVar.omcDeviceToken, onSuccess: { (response: NSHTTPURLResponse!) -> Void in
                    self.OMC_Notifications_Success()
                    }, onError: { (error: NSError!) -> Void in
                        self.OMC_Notifications_Error()
                })
            } else {
                OMC_Notifications_Success()
            }
        }

    }
    
    func OMC_Notifications_Success() -> Void {
        let username:NSString = userNameField.text!
        let password:NSString = passwordField.text!
        let prefs:NSUserDefaults = NSUserDefaults.standardUserDefaults()
        prefs.setObject(username, forKey: "USERNAME")
        prefs.setObject(password, forKey: "PASSWORD")
        prefs.setInteger(1, forKey: "ISLOGGEDIN")
        prefs.synchronize()
        //self.dismissViewControllerAnimated(true, completion: nil)
        self.performSegueWithIdentifier("goto_customerview", sender: self)
    }
    
    func OMC_Notifications_Error() -> Void {
        let alertView:UIAlertView = UIAlertView()
        alertView.title = "Sign in Failed!"
        alertView.message = "Notification Failure"
        alertView.delegate = self
        alertView.addButtonWithTitle("OK")
        alertView.show()
        }
    
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func convertDeviceTokenToString(deviceToken:NSData) -> String {
        //  Convert binary Device Token to a String (and remove the <,> and white space charaters).
        var deviceTokenStr = deviceToken.description.stringByReplacingOccurrencesOfString(">", withString: "", options: NSStringCompareOptions.LiteralSearch, range: nil)
        deviceTokenStr = deviceTokenStr.stringByReplacingOccurrencesOfString("<", withString: "", options: NSStringCompareOptions.LiteralSearch, range: nil)
        deviceTokenStr = deviceTokenStr.stringByReplacingOccurrencesOfString(" ", withString: "", options: NSStringCompareOptions.LiteralSearch, range: nil)
        
        // Our API returns token in all uppercase, regardless how it was originally sent.
        // To make the two consistent, I am uppercasing the token string here.
        deviceTokenStr = deviceTokenStr.uppercaseString
        print("Device token for push notifications: %@ ",deviceTokenStr)
        return deviceTokenStr
    }

}
