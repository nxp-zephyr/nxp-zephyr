/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/l2cap.h>
#include <zephyr/bluetooth/classic/a2dp.h>
#include "app_connect.h"

extern void app_sdp_discover_a2dp_sink(struct bt_conn *conn);
static void connected(struct bt_conn *conn, uint8_t err);
static void disconnected(struct bt_conn *conn, uint8_t reason);
static void security_changed(struct bt_conn *conn, bt_security_t level,
				 enum bt_security_err err);

struct bt_conn *default_conn[CONFIG_BT_MAX_CONN];
bt_addr_t connecting_peer_addr;
static uint8_t default_connect_initialized;

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
	.security_changed = security_changed,
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		if (default_conn[bt_conn_index(conn)] != NULL) {
			default_conn[bt_conn_index(conn)] = NULL;
		}
		printk("Connection failed (err 0x%02x)\n", err);
	} else {
		default_conn[bt_conn_index(conn)] = conn;
		if (1U == default_connect_initialized) {
			struct bt_conn_info info;

			default_connect_initialized = 0U;
			bt_conn_get_info(conn, &info);
			if (info.type == BT_CONN_TYPE_LE) {
				return;
			}

			/*
			 * Do an SDP Query on Successful ACL connection complete with the
			 * required device
			 */
			if (0 == memcmp(info.br.dst, &connecting_peer_addr, 6U)) {
				app_sdp_discover_a2dp_sink(conn);
			}
		}
		printk("Connected\n");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason 0x%02x)\n", reason);

	if (default_conn[bt_conn_index(conn)] != conn) {
		return;
	}

	if (default_conn[bt_conn_index(conn)]) {
		default_conn[bt_conn_index(conn)] = NULL;
	} else {
		return;
	}
}

static void security_changed(struct bt_conn *conn, bt_security_t level,
				 enum bt_security_err err)
{
	char addr[BT_ADDR_LE_STR_LEN];
	struct bt_conn_info info;

	bt_conn_get_info(conn, &info);
	bt_addr_to_str(info.br.dst, addr, sizeof(addr));

	if (!err) {
		printk("Security changed: %s level %u\n", addr, level);
	} else {
		printk("Security failed: %s level %u err %d\n", addr, level, err);
		if (err == BT_SECURITY_ERR_PIN_OR_KEY_MISSING) {
			bt_addr_le_t addr;
			struct bt_conn_info info;
			int ret;

			bt_conn_get_info(conn, &info);
			if (info.type == BT_CONN_TYPE_LE)
			{
				return;
			}

			printk("The peer device seems to have lost the bonding information.\n");
			printk("Delete the bonding information of the peer, please try again.\n");
			addr.type = BT_ADDR_LE_PUBLIC;
			addr.a = *info.br.dst;
			ret = bt_unpair(BT_ID_DEFAULT, &addr);
			if (ret)
			{
				printk("fail to delete.\n");
			}
			else
			{
				printk("success to delete.\n");
			}
		}
	}
}

void app_connect(uint8_t *addr)
{
	struct bt_conn *conn;

	default_connect_initialized = 1U;
	memcpy(&connecting_peer_addr, addr, 6U);
	conn = bt_conn_create_br(&connecting_peer_addr, BT_BR_CONN_PARAM_DEFAULT);
	if (!conn) {
		default_connect_initialized = 0U;
		printk("Connection failed\r\n");
	} else {
		/* unref connection obj in advance as app user */
		bt_conn_unref(conn);
		printk("Connection pending\r\n");
	}
}

void app_disconnect(void)
{
	struct bt_conn *conn = NULL;

	for (uint8_t i = 0; i < CONFIG_BT_MAX_CONN; i++) {
		if (default_conn[i] != NULL) {
			conn = default_conn[i];
			break;
		}
	}

	if (conn != NULL) {
		if (bt_conn_disconnect(conn, 0x13U)) {
			printk("Disconnection failed\r\n");
		}
	}
}

void app_connect_init(void)
{
	bt_conn_cb_register(&conn_callbacks);
}
