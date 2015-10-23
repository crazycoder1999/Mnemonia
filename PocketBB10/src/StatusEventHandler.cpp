/*
 * StatusEventHandler.cpp
 *
 *  Created on: 09/giu/2013
 *      Author: nemo
 */

#include "StatusEventHandler.h"
#include <bps/bps.h>
#include <bps/netstatus.h>
namespace cesare {

StatusEventHandler::StatusEventHandler() {
	// TODO Auto-generated constructor stub
    subscribe(netstatus_get_domain());

    bps_initialize();

    netstatus_request_events(0);
}

StatusEventHandler::~StatusEventHandler() { //ci andrebbe l'unitilize del bps .. ma giˆ lo faccio altrove.
}

void StatusEventHandler::event(bps_event_t *event) {
    bool status = false;
    const char* language = "";
    const char* country = "";
    const char* locale = "";
    const char* interface = "";
    const char* type = "none";
    netstatus_interface_details_t *details;
    if (bps_event_get_domain(event) == netstatus_get_domain()) {
    	if (NETSTATUS_INFO == bps_event_get_code(event)) {

    		status = netstatus_event_get_availability(event);
    		interface = netstatus_event_get_default_interface(event);
    		int success = netstatus_get_interface_details(interface, &details);
    		if (success == BPS_SUCCESS) {
    		    switch (netstatus_interface_get_type(details)) {

    		    case NETSTATUS_INTERFACE_TYPE_UNKNOWN:
    		        type = "Unknown";
    		        break;

    		    case NETSTATUS_INTERFACE_TYPE_WIRED:
    		        type = "Wired";
    		        break;

    		    case NETSTATUS_INTERFACE_TYPE_WIFI:
    		        type = "Wi-Fi";
    		        break;

    		    case NETSTATUS_INTERFACE_TYPE_BLUETOOTH_DUN:
    		        type = "Bluetooth";
    		        break;

    		    case NETSTATUS_INTERFACE_TYPE_USB:
    		        type = "USB";
    		        break;

    		    case NETSTATUS_INTERFACE_TYPE_VPN:
    		        type = "VPN";
    		        break;

    		    case NETSTATUS_INTERFACE_TYPE_BB:
    		        type = "BB";
    		        break;

    		    case NETSTATUS_INTERFACE_TYPE_CELLULAR:
    		        type = "Cellular";
    		        break;
    		}
    		    netstatus_free_interface_details(&details);
    		       }
    		       emit networkStatusUpdated(status, type);
    	}
    }


}

} /* namespace cesare */
