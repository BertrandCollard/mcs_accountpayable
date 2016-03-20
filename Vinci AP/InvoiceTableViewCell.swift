//
//  CustomerTableViewCell.swift
//  swift demo mcs
//
//  Created by Bertrand Collard on 02/03/2016.
//  Copyright © 2016 Bertrand Collard. All rights reserved.
//

import UIKit

class InvoiceTableViewCell: UITableViewCell {

    @IBOutlet weak var buttonWidth: NSLayoutConstraint!
    @IBOutlet weak var buttonView: UIView!
    @IBOutlet weak var supplierNameField: UILabel!
    @IBOutlet weak var dateField: UILabel!
    @IBOutlet weak var totalAmountField: UILabel!
    @IBOutlet weak var poNumberField: UILabel!
    @IBOutlet weak var statusField: UILabel!
    
    override func awakeFromNib() {
        
        super.awakeFromNib()
        // Initialization code
    }
    
    override func setSelected(selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)
        
        // Configure the view for the selected state
    }
}
