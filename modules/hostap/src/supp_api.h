/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SUPP_MGMT_H
#define ZEPHYR_SUPP_MGMT_H

#include <zephyr/net/wifi_mgmt.h>

#ifndef MAX_SSID_LEN
#define MAX_SSID_LEN 32
#endif
#ifndef MAC_ADDR_LEN
#define MAC_ADDR_LEN 6
#endif

#define MAC_STR_LEN 18 /* for ':' or '-' separated MAC address string */
#define CHAN_NUM_LEN 6 /* for space-separated channel numbers string */

#if defined CONFIG_WIFI_NM_WPA_SUPPLICANT_CRYPTO_ENTERPRISE || \
	defined CONFIG_WIFI_NM_HOSTAPD_CRYPTO_ENTERPRISE
struct wpa_supp_enterprise_file {
	uint8_t *ca_cert;
	uint32_t ca_cert_len;
	uint8_t *client_cert;
	uint32_t client_cert_len;
	uint8_t *client_key;
	uint32_t client_key_len;
	uint8_t *ca_cert2;
	uint32_t ca_cert2_len;
	uint8_t *client_cert2;
	uint32_t client_cert2_len;
	uint8_t *client_key2;
	uint32_t client_key2_len;
	uint8_t *server_cert;
	uint32_t server_cert_len;
	uint8_t *server_key;
	uint32_t server_key_len;
	uint8_t *dh_param;
	uint32_t dh_param_len;
};
#endif
/**
 * @brief Get version
 *
 * @param dev: Wi-Fi interface name to use
 * @param params: version to fill
 *
 * @return: 0 for OK; <0 for ERROR
 */
int supplicant_get_version(const struct device *dev, struct wifi_version *params);

/**
 * @brief Request a connection
 *
 * @param dev: Wi-Fi interface name to use
 * @param params: Connection details
 *
 * @return: 0 for OK; -1 for ERROR
 */
int supplicant_connect(const struct device *dev, struct wifi_connect_req_params *params);

/**
 * @brief Forces station to disconnect and stops any subsequent scan
 *  or connection attempts
 *
 * @param dev: Wi-Fi interface name to use
 *
 * @return: 0 for OK; -1 for ERROR
 */
int supplicant_disconnect(const struct device *dev);

/**
 * @brief
 *
 * @param dev: Wi-Fi interface name to use
 * @param status: Status structure to fill
 *
 * @return: 0 for OK; -1 for ERROR
 */
int supplicant_status(const struct device *dev, struct wifi_iface_status *status);

/**
 * @brief
 *
 * @param dev: Wi-Fi interface name to use
 * @param state: connection state info to fill
 *
 * @return: 0 for OK; -1 for ERROR
 */
int supplicant_wpa_state(const struct device *dev, int *state);

/**
 * @brief Request a scan
 *
 * @param dev Wi-Fi interface name to use
 * @param params Scan parameters
 * @param cb Callback to be called for each scan result
 *
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_scan(const struct device *dev, struct wifi_scan_params *params,
		    scan_result_cb_t cb);

#if defined(CONFIG_NET_STATISTICS_WIFI) || defined(__DOXYGEN__)
/**
 * @brief Get Wi-Fi statistics
 *
 * @param dev Wi-Fi interface name to use
 * @param stats Pointer to stats structure to fill
 *
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_get_stats(const struct device *dev, struct net_stats_wifi *stats);
#endif /* CONFIG_NET_STATISTICS_WIFI || __DOXYGEN__ */

#ifdef CONFIG_WIFI_NM_WPA_SUPPLICANT_ROAMING
/** Send specific scan request
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param params Scan parameters
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_spec_scan(const struct device *dev, struct wifi_scan_params *params);

/** Send 11r roaming request
 *
 * @param dev Pointer to the device structure for the driver instance.
 *
 * @return 0 if ok, < 0 if error
 */
int suppliant_11r_roaming(const struct device *dev);
#endif
/** Set 11k enable/disable
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param params 11k parameters
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_11k_enable(const struct device *dev, struct wifi_11k_params *params);

/** Send 11k neighbor request
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param params 11k parameters
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_11k_neighbor_request(const struct device *dev, struct wifi_11k_params *params);

/**
 * @brief Set Wi-Fi power save configuration
 *
 * @param dev Wi-Fi interface name to use
 * @param params Power save parameters to set
 *
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_set_power_save(const struct device *dev, struct wifi_ps_params *params);

/**
 * @brief Set Wi-Fi TWT parameters
 *
 * @param dev Wi-Fi interface name to use
 * @param params TWT parameters to set
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_set_twt(const struct device *dev, struct wifi_twt_params *params);

/**
 * @brief Set Wi-Fi BTWT parameters
 *
 * @param dev Wi-Fi interface name to use
 * @param params BTWT parameters to set
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_set_btwt(const struct device *dev, struct wifi_twt_params *params);

/** Set rts threshold
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param rts_threshold rts threshold to set
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_set_rts_threshold(const struct device *dev, unsigned int rts_threshold);

/**
 * @brief Get Wi-Fi power save configuration
 *
 * @param dev Wi-Fi interface name to use
 * @param config Address of power save configuration to fill
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_get_power_save_config(const struct device *dev, struct wifi_ps_config *config);

/**
 * @brief Set Wi-Fi Regulatory domain
 *
 * @param dev Wi-Fi interface name to use
 * @param reg_domain Regulatory domain to set
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_reg_domain(const struct device *dev, struct wifi_reg_domain *reg_domain);

/**
 * @brief Set Wi-Fi mode of operation
 *
 * @param dev Wi-Fi interface name to use
 * @param mode Mode setting to set
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_mode(const struct device *dev, struct wifi_mode_info *mode);

/**
 * @brief Set Wi-Fi packet filter for sniffing operation
 *
 * @param dev Wi-Fi interface name to use
 * @param filter Filter settings to set
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_filter(const struct device *dev, struct wifi_filter_info *filter);

/**
 * @brief Set Wi-Fi channel for monitor or TX injection mode
 *
 * @param dev Wi-Fi interface name to use
 * @param channel Channel settings to set
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_channel(const struct device *dev, struct wifi_channel_info *channel);

/** Send bss transition query
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param reason query reason
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_btm_query(const struct device *dev, uint8_t reason);

/** Send legacy roam
 *
 * @param dev Pointer to the device structure for the driver instance.
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_legacy_roam(const struct device *dev);

/** Judge ap whether support the capability
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param capab is the capability to judge
 *
 * @return 1 if support, 0 if not support
 */
int supplicant_bss_ext_capab(const struct device *dev, int capab);

/** Start wps pbc connection
 *
 * @param dev Pointer to the device structure for the driver instance.
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_wps_pbc(const struct device *dev);

/** Set or get wps pin
 *
 * @param dev Pointer to the device structure for the driver instance
 * @param params wps pin operarion and pin number
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_wps_pin(const struct device *dev, struct wifi_wps_pin_params *params);

/** Flush PMKSA cache entries
 *
 * @param dev Pointer to the device structure for the driver instance.
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_pmksa_flush(const struct device *dev);

/** Get Wi-Fi connection paramters recently used
 *
 * @return pointer to the Wi-Fi connection paramters
 */
struct wifi_connect_req_params *supplicant_get_wifi_conn_params(void);

#if defined CONFIG_WIFI_NM_WPA_SUPPLICANT_CRYPTO_ENTERPRISE || \
	defined CONFIG_WIFI_NM_HOSTAPD_CRYPTO_ENTERPRISE
/** Set Wi-Fi enterprise mode CA/client Cert and key file
 *
 * @param file Pointer to the CA/client Cert and key file.
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_add_enterprise_file(struct wpa_supp_enterprise_file *file);
#endif
#ifdef CONFIG_AP
#ifdef CONFIG_WIFI_NM_HOSTAPD_AP
/**
 * @brief Get Wi-Fi AP Status
 *
 * @param dev Wi-Fi device
 * @param params AP status
 * @return 0 for OK; -1 for ERROR
 */
int hapd_state(const struct device *dev, int *state);

/**
 * @brief Set/Get Wi-Fi AP Bandwidth
 *
 * @param dev Wi-Fi device
 * @param params AP parameters
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_ap_bandwidth(const struct device *dev, struct wifi_ap_config_params *params);

/**
 * @brief Get Wi-Fi SAP status
 *
 * @param dev Wi-Fi device
 * @param status SAP status
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_ap_status(const struct device *dev, struct wifi_iface_status *status);

#ifdef CONFIG_WIFI_NM_HOSTAPD_WPS
/** Start AP wps pbc connection
 *
 * @param dev Pointer to the device structure for the driver instance.
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_ap_wps_pbc(const struct device *dev);

/** Set or get AP wps pin
 *
 * @param dev Pointer to the device structure for the driver instance
 * @param params wps pin operarion and pin number
 *
 * @return 0 if ok, < 0 if error
 */
int supplicant_ap_wps_pin(const struct device *dev, struct wifi_wps_pin_params *params);
#endif

/**
 * @brief Wi-Fi AP configuration parameter.
 *
 * @param dev Wi-Fi device
 * @param params AP parameters
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_ap_config_params(const struct device *dev, struct wifi_ap_config_params *params);
#endif

/**
 * @brief Set Wi-Fi AP configuration
 *
 * @param dev Wi-Fi interface name to use
 * @param params AP configuration parameters to set
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_ap_enable(const struct device *dev,
			 struct wifi_connect_req_params *params);

/**
 * @brief Disable Wi-Fi AP
 * @param dev Wi-Fi interface name to use
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_ap_disable(const struct device *dev);

/**
 * @brief Set Wi-Fi AP STA disconnect
 * @param dev Wi-Fi interface name to use
 * @param mac_addr MAC address of the station to disconnect
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_ap_sta_disconnect(const struct device *dev,
				 const uint8_t *mac_addr);

#endif /* CONFIG_AP */

#ifdef CONFIG_WIFI_NM_WPA_SUPPLICANT_DPP
/**
 * @brief Dispatch DPP operations for STA
 *
 * @param dev Wi-Fi interface name to use
 * @param dpp_params DPP action enum and params in string
 * @return 0 for OK; -1 for ERROR
 */
int supplicant_dpp_dispatch(const struct device *dev,
			    struct wifi_dpp_params *params);

#ifdef CONFIG_WIFI_NM_HOSTAPD_AP
/**
 * @brief Dispatch DPP operations for AP
 *
 * @param dev Wi-Fi interface name to use
 * @param dpp_params DPP action enum and params in string
 * @return 0 for OK; -1 for ERROR
 */
int hapd_dpp_dispatch(const struct device *dev,
		      struct wifi_dpp_params *params);
#endif /* CONFIG_WIFI_NM_HOSTAPD_AP */
#endif /* CONFIG_WIFI_NM_WPA_SUPPLICANT_DPP */
#endif /* ZEPHYR_SUPP_MGMT_H */
