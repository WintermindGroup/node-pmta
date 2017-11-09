#include "pmta.h"

using namespace pmta::submitter;

/*
 * PMTAConnection
 */

Nan::Persistent<v8::Function> PMTAConnection::constructor;

PMTAConnection::PMTAConnection (const char *pHost, int pPort,
  const char *pName, const char *pPassword)
  : mHost(pHost), mPort(pPort), mName(pName), mPassword(pPassword) {
  mConnection = new pmta::submitter::Connection(mHost, mPort, mName,
    mPassword);
}

void PMTAConnection::Init (v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PMTAConnection").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl,  "submit",     submit);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("PMTAConnection").ToLocalChecked(),
    tpl->GetFunction());
}

PMTAConnection::~PMTAConnection() {
  delete mConnection;
}

void PMTAConnection::New (const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError(
      Nan::Error("Use the `new` operator to create PMTAConnection"));
  }
    
  if (info.Length() < 2) {
    Nan::ThrowError(Nan::Error("Connection(host, port, [name], [password]"));
  }

  if (!info[0]->IsString()) {
    Nan::ThrowError(Nan::Error("Connection(): `host` must be a string"));
  }

  if (!info[1]->IsInt32()) {
    Nan::ThrowError(
      Nan::Error("Connection(): `port` argument must be an integer"));
  }

  v8::String::Utf8Value pHost(info[0]->ToString());
  const char *host  = strdup(*pHost);
  int port          = info[1]->ToInteger()->Value();

  const char *name;
  const char *password;

  if (info[2]->IsUndefined()) {
    name = "";
  } else {
    v8::String::Utf8Value pName(info[2]->ToString());
    name = strdup(*pName);
  }

  if (info[3]->IsUndefined()) {
    password = "";
  } else {
    v8::String::Utf8Value pPassword(info[3]->ToString());
    password = strdup(*pPassword);
  }

  PMTAConnection *obj = new PMTAConnection(host, port, name, password);
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

void PMTAConnection::submit (const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() < 1) {
    Nan::ThrowError(Nan::Error("submitSync(message): missing argument"));
  }

  PMTAConnection* connection = 
    ObjectWrap::Unwrap<PMTAConnection>(info.Holder());
  PMTAMessage*    message    = 
    ObjectWrap::Unwrap<PMTAMessage>(info[0]->ToObject());

  v8::Local<v8::Object> ret = Nan::New<v8::Object>();
  try {
    connection->mConnection->submit(*message->mMessage);
    Nan::Set(ret, Nan::New("submitted").ToLocalChecked(), Nan::New(true));
  } catch (std::exception& e) {
    Nan::Set(ret, Nan::New("submitted").ToLocalChecked(), Nan::New(false));
    Nan::Set(ret, Nan::New("errorMessage").ToLocalChecked(), 
      Nan::New(e.what()).ToLocalChecked());
  }
  info.GetReturnValue().Set(ret);
}

/* 
 * PMTAMessage
 */
Nan::Persistent<v8::Function> PMTAMessage::constructor;

PMTAMessage::PMTAMessage (const char* psender) : mSender(psender) {
  mMessage = new pmta::submitter::Message(mSender);
}

PMTAMessage::~PMTAMessage (void) {
  delete mMessage;
}

void PMTAMessage::Init (v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PMTAMessage").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "sender",        sender);
  Nan::SetPrototypeMethod(tpl, "addData",       addData);
  Nan::SetPrototypeMethod(tpl, "setVerp",       setVerp);
  Nan::SetPrototypeMethod(tpl, "setJobId",      setJobId);
  Nan::SetPrototypeMethod(tpl, "beginPart",     beginPart);
  Nan::SetPrototypeMethod(tpl, "setEncoding",   setEncoding);
  Nan::SetPrototypeMethod(tpl, "addRecipient",  addRecipient);
  Nan::SetPrototypeMethod(tpl, "addMergeData",  addMergeData);
  Nan::SetPrototypeMethod(tpl, "setReturnType", setReturnType);
  Nan::SetPrototypeMethod(tpl, "setEnvelopeId", setEnvelopeId);
  Nan::SetPrototypeMethod(tpl, "setVirtualMta", setVirtualMta);
  Nan::SetPrototypeMethod(tpl, "addDateHeader", addDateHeader);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("PMTAMessage").ToLocalChecked(),
    tpl->GetFunction());
}

void PMTAMessage::New (const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError(Nan::TypeError(
      "Use the `new` operator to create PMTAMessage")
    );
  }

  if (!info[0]->IsString()) {
    Nan::ThrowError(Nan::TypeError(
      "PMTAMessage(string address): `address` must be a string"));
  }

  v8::String::Utf8Value param1(info[0]->ToString());
  const char* sender = strdup(*param1);

  try {
    PMTAMessage* obj = new PMTAMessage(sender);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } catch (std::exception& e) {
    Nan::ThrowError(Nan::TypeError(Nan::New(e.what()).ToLocalChecked()));
  }
}

void PMTAMessage::sender (const Nan::FunctionCallbackInfo<v8::Value>& info) {
  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  info.GetReturnValue().Set(
    Nan::New<v8::String>(obj->mSender).ToLocalChecked());
}

void PMTAMessage::setVerp (const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (!info[0]->IsBoolean() || info.Length() < 1) {
    Nan::ThrowError(Nan::TypeError("setVerp(true|false)"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  v8::Local<v8::Boolean> param0(info[0]->ToBoolean());
  bool verp = param0->BooleanValue();
  obj->mMessage->setVerp(verp);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::setEncoding
  (const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (!info[0]->IsString() || info.Length() < 1) {
    Nan::ThrowError(Nan::Error("setEncoding(String encoding)"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  v8::String::Utf8Value psetEncoding(info[0]->ToString());
  const char *set_encoding(*psetEncoding);

  PmtaMsgENCODING encoding;

  if (strcmp(set_encoding, "ENCODING_7BIT") == 0) {
    encoding = PmtaMsgENCODING_7BIT;
  } else if (strcmp(set_encoding, "ENCODING_8BIT") == 0) {
    encoding = PmtaMsgENCODING_8BIT;
  } else if (strcmp(set_encoding, "ENCODING_BASE64") == 0) {
    encoding = PmtaMsgENCODING_BASE64;
  } else {
    encoding = PmtaMsgENCODING_7BIT;
  }

  obj->mMessage->setEncoding(encoding);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::setJobId (const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (!info[0]->IsString() || info.Length() < 1) {
    Nan::ThrowError(Nan::Error("setJobId(String jobid)"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  v8::String::Utf8Value param1(info[0]->ToString());
  const char* jobid = strdup(*param1);

  obj->mMessage->setJobId(jobid);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::setReturnType (
  const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (!info[0]->IsString() || info.Length() < 1) {
  Nan::ThrowError(Nan::Error("setReturnType(PmtaMsgRETURN returnType)"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  v8::String::Utf8Value param1(info[0]->ToString());
  char* cmp = strdup(*param1);

  PmtaMsgRETURN ret;

  if (strcmp(cmp,"RETURN_FULL") == 0) {
    ret = PmtaMsgRETURN_FULL;
  } else if (strcmp(cmp,"RETURN_HEADERS") == 0) {
    ret = PmtaMsgRETURN_HEADERS;
  } else {
    ret = PmtaMsgRETURN_HEADERS;
  }

  obj->mMessage->setReturnType(ret);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::setEnvelopeId (
  const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (!info[0]->IsString() || info.Length() < 1) {
    Nan::ThrowError(Nan::Error("setEnvelopeId(String envelopeId)"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  v8::String::Utf8Value param1(info[0]->ToString());
  const char* eid = strdup(*param1);

  obj->mMessage->setEnvelopeId(eid);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::setVirtualMta (
  const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (!info[0]->IsString() || info.Length() < 1) {
  Nan::ThrowError(Nan::Error("setVirtualMta(String vmta)"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  v8::String::Utf8Value param1(info[0]->ToString());
  const char* vmta = strdup(*param1);

  obj->mMessage->setVirtualMta(vmta);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::beginPart(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (!info[0]->IsInt32() || info.Length() < 1) {
  Nan::ThrowError(Nan::Error("beginPart(Int part)"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  int part = info[0]->ToInteger()->Value();

  if (part <= 1) {
  Nan::ThrowError(
    Nan::Error("beginPart(Int part): `part` must be greater than 1"));
  }
  obj->mMessage->beginPart(part);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::addData (const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() < 2) {
    Nan::ThrowError(
      Nan::Error("addData(String data, Int len): insufficient arguments"));
  }

  if (!info[0]->IsString()) {
    Nan::ThrowError(
      Nan::Error("addData(String data, Int len): `data` must be a string"));
  }

  if (!info[1]->IsInt32()) {
    Nan::ThrowError(
      Nan::Error("addData(String data, Int len): `len` must be an integer"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  v8::String::Utf8Value param1(info[0]->ToString());
  const char* data = strdup(*param1);
  int length = info[1]->ToInteger()->Value();

  obj->mMessage->addData(data, length);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::addMergeData (
  const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() < 2) {
    Nan::ThrowError(
      Nan::Error("addData(String data, Int len): insufficient arguments"));
  }

  if (!info[0]->IsString()) {
    Nan::ThrowError(
      Nan::Error("addData(String data, Int len): `data` must be a string"));
  }

  if (!info[1]->IsInt32()) {
    Nan::ThrowError(
      Nan::Error("addData(String data, Int len): `len` must be an integer"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  v8::String::Utf8Value param1(info[0]->ToString());
  const char* data = strdup(*param1);
  int length = info[1]->ToInteger()->Value();

  obj->mMessage->addMergeData(data, length);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::addDateHeader (
  const Nan::FunctionCallbackInfo<v8::Value>& info) {

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  obj->mMessage->addDateHeader();

  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTAMessage::addRecipient (
  const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() < 1) {
  Nan::ThrowError(Nan::Error("addRecipient(Recipient recipient)"));
  }

  PMTAMessage* obj = ObjectWrap::Unwrap<PMTAMessage>(info.Holder());
  PMTARecipient* robj = ObjectWrap::Unwrap<PMTARecipient>(info[0]->ToObject());

  obj->mMessage->addRecipient(*robj->mRecipient);

  info.GetReturnValue().Set(Nan::Undefined());
}

/*
 * PMTARecipient
 */
Nan::Persistent<v8::Function> PMTARecipient::constructor;

PMTARecipient::PMTARecipient (const char* pAddress) : mAddress(pAddress) {
  mRecipient = new Recipient(mAddress);
}

PMTARecipient::~PMTARecipient (void) {
  delete mRecipient;
}
 
void PMTARecipient::Init (v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PMTARecipient").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl,  "address",          address);
  Nan::SetPrototypeMethod(tpl,  "defineVariable",   defineVariable);
  Nan::SetPrototypeMethod(tpl,  "setNotify",        setNotify);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("PMTARecipient").ToLocalChecked(),
    tpl->GetFunction());
}

void PMTARecipient::New (const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (!info.IsConstructCall()) {
    Nan::ThrowError(Nan::TypeError(
      "Use the `new` operator to create PMTARecipient")
    );
  }

  if (!info[0]->IsString()) {
    Nan::ThrowError(Nan::TypeError(
      "PMTARecipient(string address): `address` must be a string"));
  }

  v8::String::Utf8Value param1(info[0]->ToString());
  const char* addr = strdup(*param1);

  try {
    PMTARecipient* obj = new PMTARecipient(addr);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } catch (std::exception& e) {
    Nan::ThrowError(Nan::TypeError(Nan::New(e.what()).ToLocalChecked()));
  }
}

void PMTARecipient::address (const Nan::FunctionCallbackInfo<v8::Value>& info) {
  PMTARecipient* obj = ObjectWrap::Unwrap<PMTARecipient>(info.Holder());
  info.GetReturnValue().Set(
    Nan::New<v8::String>(obj->mAddress).ToLocalChecked());
}

void PMTARecipient::setNotify (
  const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() < 1) {
    Nan::ThrowError(Nan::TypeError("setNotify(PmtaRcptNOTIFY)"));
  }

  if (!info[0]->IsNumber()) {
    Nan::ThrowError(Nan::TypeError("Argument must be a number"));
  }

  PMTARecipient* obj  = ObjectWrap::Unwrap<PMTARecipient>(info.Holder());
  int notify_when     = info[0]->ToInteger()->Value();

  obj->mRecipient->setNotify(notify_when);
  info.GetReturnValue().Set(Nan::Undefined());
}

void PMTARecipient::defineVariable (
  const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() < 2) {
    Nan::ThrowError(Nan::TypeError("defineVariable(name, value)"));
  }

  if (!info[0]->IsString()) {
    Nan::ThrowError(Nan::TypeError("`name` must be a string"));
  }
    
  if (!info[1]->IsString()) {
    Nan::ThrowError(Nan::TypeError("`value` must be a string"));
  }
    
  PMTARecipient* obj    = ObjectWrap::Unwrap<PMTARecipient>(info.Holder());
  v8::String::Utf8Value pName(info[0]->ToString());
  v8::String::Utf8Value pValue(info[1]->ToString());

  char* name  = strdup(*pName);
  char* value = strdup(*pValue);

  obj->mRecipient->defineVariable(name, value);
  info.GetReturnValue().Set(Nan::Undefined());
}

void RegisterModule (v8::Local<v8::Object> exports) {
  PMTAMessage::Init(exports);
  PMTARecipient::Init(exports);
  PMTAConnection::Init(exports);
}

NODE_MODULE(pmta, RegisterModule);
