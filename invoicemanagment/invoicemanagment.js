module.exports = function(service) {
	var sha1 = require('sha1');

	/**
	 *  The file samples.txt in the archive that this file was packaged with contains some example code.
	 */


	service.put('/mobile/custom/invoicemanagment/invoice/:id', function(req,res) {
		var result = {};
		var body = {
			"Header": null,
			"Body": {
				"setStatus": {
					"invoiceId": parseInt(req.params.id),
					"status": req.body.status
				}
			}
		}
		console.info(JSON.stringify(body)); 
		console.info("req.headers['if-match']:" + req.headers["if-match"]);
		var ifMatch = req.headers;
		if ((typeof req.headers["if-match"] === 'undefined') || (req.headers["if-match"] == '*')) {
			req.oracleMobile.connectors.post('InvoiceWS', 'setStatus', body, {inType: 'json'}).then(
				function (result) {
					console.info("result is: " + result.result);
					var obj = JSON.parse(result.result);
					res.set('Oracle-Mobile-Sync-Resource-Type', 'item');
					req.oracleMobile.sync.setItem( 
						res,
						obj.Body.setStatusResponse.return
					);
					res.end();
					//res.send(200, obj.Body.setStatusResponse.return);
				},
				function (error) {
					console.info("error is: " + error.statusCode);
					res.send(500, error.error); 
				}
			);
		} else {
			var matchEtag = req.headers["if-match"];
			var bodyGet = {
				"Header": null,
				"Body": {
					"getInvoiceById": {
						"arg0": parseInt(req.params.id)
					}
				}
			}
			req.oracleMobile.connectors.post('InvoiceWS', 'getInvoiceById', bodyGet, {inType: 'json'}).then(
			function (result) {
				console.info("result is: " + result.result);
				var obj = JSON.parse(result.result);
				var refsha1 = sha1(JSON.stringify(obj.Body.getInvoiceByIdResponse.return));
				var getInvoiceByIdResponse = JSON.stringify(obj.Body.getInvoiceByIdResponse.return);
				console.info("getInvoiceByIdResponse(" + req.params.id + "):" + getInvoiceByIdResponse);
				console.info("sha1 getInvoiceById(" + req.params.id + "):" + refsha1);
				if (refsha1 != matchEtag) {
					//res.send(412, JSON.parse(getCustomerByIdResponse));
					res.send(412);
				} else {
					req.oracleMobile.connectors.post('InvoiceWS', 'setStatus', body, {inType: 'json'}).then(
						function (result) {
							console.info("result is: " + result.result);
							var obj = JSON.parse(result.result);
							res.set('Oracle-Mobile-Sync-Resource-Type', 'item');
							req.oracleMobile.sync.setItem( 
								res,
								obj.Body.setStatusResponse.return
								);
							res.end();
							//res.send(200, obj.Body.setStatusResponse.return);
						},
						function (error) {
							console.info("error is: " + error.statusCode);
							res.send(500, error.error); 
						}
					);
				}
			},
			function (error) {
				console.info("error is: " + error.statusCode);
				res.send(500, error.error); 
			}
		);
		}
	});


	service.delete('/mobile/custom/invoicemanagment/invoice/:id', function(req,res) {
		var result = {};
		var body = {
			"Header": null,
			"Body": {
				"removeInvoice": {
					"arg0": {
						"invoiceId": parseInt(req.params.id)
					}
				}
			}
		}
		console.info(JSON.stringify(body)); 
		var ifMatch = req.headers;
		if ((typeof req.headers["if-match"] === 'undefined') || (req.headers["if-match"] == '*')) {
			req.oracleMobile.connectors.post('InvoiceWS', 'removeInvoice', body, {inType: 'json'}).then(
			function (result) {
				console.info("result is: " + result.statusCode);
				res.send(200, result);
			},
			function (error) {
				console.info("error is: " + error.statusCode);
				res.send(500, error.error); 
			}
		);
		} else {
			// exist
			var matchEtag = req.headers["if-match"];
			var bodyGet = {
				"Header": null,
				"Body": {
					"getInvoiceById": {
						"arg0": parseInt(req.params.id)
					}
				}
			}
			req.oracleMobile.connectors.post('InvoiceWS', 'getInvoiceById', bodyGet, {inType: 'json'}).then(
			function (result) {
				console.info("result is: " + result.statusCode);
				var obj = JSON.parse(result.result);
				var getInvoiceByIdResponse = JSON.stringify(obj.Body.getinvoiceByIdResponse.return);
				var refsha1 = sha1(getInvoiceByIdResponse);
				if (refsha1 != matchEtag) {
					//res.send(412, getCustomerByIdResponse);
					res.send(412);
				} else {
					req.oracleMobile.connectors.post('InvoiceWS', 'removeInvoice', body, {inType: 'json'}).then(
						function (result) {
							console.info("result is: " + result.statusCode);
							res.send(200, result);
						},
						function (error) {
							console.info("error is: " + error.statusCode);
							res.send(500, error.error); 
						}
					);
				}
			},
			function (error) {
				console.info("error is: " + error.statusCode);
				res.send(500, error.error); 
			}
		);
		}
			
	});

	service.get('/mobile/custom/invoicemanagment/invoice/:id', function(req,res) {
		var result = {};
		var bodyGet = {
			"Header": null,
			"Body": {
				"getInvoiceById": {
					"arg0": parseInt(req.params.id)
				}
			}
		}
		req.oracleMobile.connectors.post('InvoiceWS', 'getInvoiceById', bodyGet, {inType: 'json'}).then(
			function (result) {
				console.info("result is: " + result.result);
				var obj = JSON.parse(result.result);
				var getInvoiceByIdResponse = JSON.stringify(obj.Body.getInvoiceByIdResponse);
				console.info("getInvoiceById(" + req.params.id + "):" + getInvoiceByIdResponse);
				if (getInvoiceByIdResponse != 'null' && getInvoiceByIdResponse != 'undefined') {
					res.set('Oracle-Mobile-Sync-Resource-Type', 'item');
					req.oracleMobile.sync.setItem( 
						res,
						obj.Body.getInvoiceByIdResponse.return
					);
					res.end();
					//res.send(200, obj.Body.getInvoiceByIdResponse.return);
				} else {
					res.send(404);
				}
			},
			function (error) {
				console.info("error is: " + error.statusCode);
				res.send(500, error.error); 
			}
		);
	});

	service.get('/mobile/custom/invoicemanagment/invoices', function(req,res) {
		var result = {};
		
		var body = { 
			"Header": null, 
			"Body": {
				"getInvoiceFindAll": null
			}
		};
		console.info(JSON.stringify(body)); 
		req.oracleMobile.connectors.post('InvoiceWS', 'getInvoiceFindAll', body, {inType: 'json'}).then(
			function (result) {
				console.info("result is: " + result.statusCode);
				var obj = JSON.parse(result.result);
				for(var k in obj.Body.getInvoiceFindAllResponse.return) {
					var invoiceObj = obj.Body.getInvoiceFindAllResponse.return[k];
					req.oracleMobile.sync.addItem(
						res,
						invoiceObj,
						"/mobile/custom/invoicemanagment/invoice/"+invoiceObj.invoiceId,
						sha1(JSON.stringify(invoiceObj))
						);
    
				}
				res.end();
			},
			function (error) {
				console.info("error is: " + error.statusCode);
				res.send(500, error.error); 
			}
		);
	});

};
