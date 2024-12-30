/*
 * Copyright (c) 2024 Gaël PORTAY
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

int main(void)
{
	struct net_if *iface = net_if_get_first_by_type(&NET_L2_GET_NAME(ETHERNET));
	return net_mgmt(NET_REQUEST_WIFI_CONNECT_STORED, iface, NULL, 0);
}
