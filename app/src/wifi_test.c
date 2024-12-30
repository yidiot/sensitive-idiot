/*
 * Copyright (c) 2024-2025 Gaël PORTAY
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

K_SEM_DEFINE(net_ipv4_sem, 0, 1);

static struct net_mgmt_event_callback net_mgmt_event_callback;
static void net_mgmt_event_handler(struct net_mgmt_event_callback *cb,
				   uint32_t mgmt_event,
				   struct net_if *iface)
{
	int i;

	if (mgmt_event != NET_EVENT_IPV4_ADDR_ADD)
		return;

	for (i = 0; i < NET_IF_MAX_IPV4_ADDR; i++)
		if (iface->config.ip.ipv4->unicast[i].ipv4.addr_type ==
								  NET_ADDR_DHCP)
			break;

	if (i < NET_IF_MAX_IPV4_ADDR)
		k_sem_give(&net_ipv4_sem);
}

int main(void)
{
	struct net_if *iface = net_if_get_first_by_type(&NET_L2_GET_NAME(ETHERNET));
	net_mgmt_init_event_callback(&net_mgmt_event_callback,
				     net_mgmt_event_handler,
				     NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&net_mgmt_event_callback);
	net_mgmt(NET_REQUEST_WIFI_CONNECT_STORED, iface, NULL, 0);
	return k_sem_take(&net_ipv4_sem, K_FOREVER);
}
