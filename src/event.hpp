#pragma once

#include "furniture/furniture.hpp"
#include "furniture/stove.hpp"
#include "furniture/table.hpp"
#include "furniture/chair.hpp"
#include "furniture/counter.hpp"
#include "furniture/stand.hpp"
#include "staff/staff.hpp"
#include "staff/chef.hpp"
#include "staff/waiter.hpp"
#include "customer.hpp"

#define NUM_EVENTS 4

enum class EventType {
	CUSTOMER_ARRIVE,
	WAITER_SIGNAL,
	CUSTOMER_ORDER,
	ORDER_IN
};

struct Event {
	EventType type;
};

struct CustomerArriveEvent : public Event {
	Customer * customer;
};

struct WaiterSignalEvent : public Event {
	Customer * customer;
	Waiter * waiter;
	Table * table;
};

struct CustomerOrderEvent : public Event {
	Customer * customer;
	// Include the order data in here
};

struct OrderInEvent : public Event {
	Customer * customer;
};