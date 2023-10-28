/*
 * Copyright (C) 2022-2023 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_

#include <string>
#include <vector>
#include <sys/_system_properties.h>

using android::base::GetProperty;

/*
 * SetProperty does not allow updating read only properties and as a result
 * does not work for our use case. Write "OverrideProperty" to do practically
 * the same thing as "SetProperty" without this restriction.
 */
void OverrideProperty(const char* name, const char* value) {
    size_t valuelen = strlen(value);

    prop_info* pi = (prop_info*)__system_property_find(name);
    if (pi != nullptr) {
        __system_property_update(pi, value, valuelen);
    } else {
        __system_property_add(name, strlen(name), value, valuelen);
    }
}

// Function to override or add a property
void property_override(const std::string& prop, const std::string& value, bool add = true) {
    prop_info* pi = (prop_info*)__system_property_find(prop.c_str());
    if (pi != nullptr) {
        __system_property_update(pi, value.c_str(), value.length());
    } else if (add) {
        __system_property_add(prop.c_str(), prop.length(), value.c_str(), value.length());
    }
}

const std::vector<std::string> ro_props_default_source_order = {
    "bootimage.", "odm.", "product.", "system.",
    "system_ext.", "vendor.", "vendor_dlkm.", "",
};

// Common properties override for Realme 9 Pro
void apply_realme_properties() {

    for (const auto& source : ro_props_default_source_order) {
        property_override("ro.product." + source + "brand", "realme", true);
        property_override("ro.product." + source + "device", "RE54CBL1", true);
        property_override("ro.product." + source + "manufacturer", "realme", true);
        property_override("ro.product." + source + "name", "RE54CBL1", true);
        property_override("ro." + source + "build.fingerprint",
            "realme/RMX3471/RE54CBL1:13/TP1A.220905.001/S.13f10a3-10d8e-82f8b:user/release-keys", true);
    }

    OverrideProperty("ro.build.description", "RE54CBL1-user 13 TP1A.220905.001 S.13f10a3-10d8e-82f8b release-keys");
    OverrideProperty("ro.build.product", "RE54CBL1");
    OverrideProperty("ro.com.google.clientidbase", "android-oppo");
    OverrideProperty("bluetooth.device.default_name", "Realme 9 Pro");
    OverrideProperty("vendor.usb.product_string", "Realme 9 Pro");
}

// Vendor-specific property overrides
void vendor_load_properties() {
    auto prjname = std::stoi(GetProperty("ro.boot.prjname", "0"));
    auto sku = std::stoi(GetProperty("ro.boot.product.hardware.sku", "0"));

    for (const auto& source : ro_props_default_source_order) {
        switch (prjname) {
            // Realme 9 Pro
            case 21707: // IN
                apply_realme_properties();
                property_override("ro.product." + source + "model", "RMX3471", true);
                break;
            case 21708: // EU
                apply_realme_properties();
                property_override("ro.product." + source + "model", "RMX3472", true);
                break;
            case 136747: // CN
                apply_realme_properties();
                property_override("ro.product." + source + "model", "RMX3478", true);
                break;
            default:
                LOG(ERROR) << "Unexpected project name: " << prjname;
        }
    }

    switch (sku) {
        // OnePlus Nord CE 2 Lite
        case 2: // IN
            OverrideProperty("ro.product.product.model", "CPH2381");
            break;
        case 6: // GL
            OverrideProperty("ro.product.product.model", "CPH2409");
            break;
        default:
            LOG(ERROR) << "Unexpected SKU: " << sku;
    }
}
