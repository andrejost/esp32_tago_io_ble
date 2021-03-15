
'use strict';
const Analysis = require('tago/analysis');
const Utils    = require('tago/utils');
const Device   = require('tago/device');
 
function run_analysis(context) {
 
    //Register the Device ID in the "Environment Variables Session of the Anlysis"
    const env_vars  = Utils.env_to_obj(context.environment);
 
    //Creates a new device based on the DEVICE_TOKEN variable
    const my_device = new Device(env_vars.device_token);
     
    //In the device, it looks for the variable "semaforo" and the last created item in the Bucket
    my_device.find({"variable":"semaforo", "query":"last_item"}).then((result_array) => {
        if (!result_array[0]) {
            return context.log("Nothing found in the device");
        }
        
        //Returns the last received in a temporary variable
        const value = result_array[0].value;
 
        //Makes a transformation to the data received in the bucket so it can be shown accordingly
        if (value == "1") {
          const obj_to_save = {
              "variable": "semaforo",
              "value": ("AMARELO")
              };
              context.log(obj_to_save);
              //Insert data in the Device Bucket
              my_device.insert(obj_to_save).then(context.log("OK, it works!")).catch(error => context.log("Error to insert", error));
        };
        if (value == "2") {
          const obj_to_save = {
              "variable": "semaforo",
              "value": ("VERDE")
              };
              context.log(obj_to_save);
              //Insert data in the Device Bucket
              my_device.insert(obj_to_save).then(context.log("OK, it works!")).catch(error => context.log("Error to insert", error));
              
        };
        if (value == "3") {
          const obj_to_save = {
              "variable": "semaforo",
              "value": ("VERMELHO")
              };
              context.log(obj_to_save);
              //Insert data in the Device Bucket
              my_device.insert(obj_to_save).then(context.log("OK, it works!")).catch(error => context.log("Error to insert", error));
        };
      }).catch(context.log);
}
module.exports = new Analysis(run_analysis, 'REPLACE HERE WITH THE ID OF YOUR ANALYSIS');