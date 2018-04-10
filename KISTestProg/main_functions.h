#pragma once

#include "stopper.h"
#include "common.h"
class Request;

Request* GetRequest(Stopper stopSignal);

void ProcessRequest(Request* request, Stopper stopSignal);

void DeleteRequest(Request* request);