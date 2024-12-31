/*
 * Copyright (c) 2024-2025 Gaël PORTAY
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

static struct k_poll_signal net_dhcp4_sig;
struct k_poll_event events[1] = {
	K_POLL_EVENT_INITIALIZER(K_POLL_TYPE_SIGNAL,
				 K_POLL_MODE_NOTIFY_ONLY,
				 &net_dhcp4_sig),
};

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
		k_poll_signal_raise(&net_dhcp4_sig, 0x41);
}

int main(void)
{
	struct net_if *iface;
	int signaled, result;
	iface = net_if_get_first_by_type(&NET_L2_GET_NAME(ETHERNET));
	net_mgmt_init_event_callback(&net_mgmt_event_callback,
				     net_mgmt_event_handler,
				     NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&net_mgmt_event_callback);
	net_mgmt(NET_REQUEST_WIFI_CONNECT_STORED, iface, NULL, 0);
	k_poll_signal_init(&net_dhcp4_sig);
	k_poll(events, 1, K_FOREVER);
	k_poll_signal_check(&net_dhcp4_sig, &signaled, &result);
	return !signaled || result != 0x41;
}
