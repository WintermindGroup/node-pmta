/* Copyright (C) 2015  Dan Nielsen <dnielsen@reachmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PMTA_H 
#define PMTA_H

#include <node.h>
#include <string.h>

#include "submitter/Message.hxx"
#include "submitter/Recipient.hxx"
#include "submitter/Connection.hxx"

class PMTAConnection : public node::ObjectWrap {
  
  public:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static void Init(v8::Handle<v8::Object> target);
    pmta::submitter::Connection* connection_;

  protected:
    PMTAConnection (const char* host, int port, const char* name = "",
        const char* password = "");

    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    static v8::Handle<v8::Value> Submit(const v8::Arguments& args);
    static v8::Handle<v8::Value> SubmitSync(const v8::Arguments& args);
    static void AsyncSubmitWork(uv_work_t* req);
    static void AsyncSubmitAfter(uv_work_t* req);

    const char* host_;
    int port_;
    const char* name_;
    const char* password_;

  private:
    ~PMTAConnection();
};

class PMTAMessage : public node::ObjectWrap {

  public:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static void Init(v8::Handle<v8::Object> target);
    pmta::submitter::Message* message_;

  protected:
    PMTAMessage (const char* sender);

    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    static v8::Handle<v8::Value> Sender(const v8::Arguments& args);
    static v8::Handle<v8::Value> AddData(const v8::Arguments& args);
    static v8::Handle<v8::Value> SetVerp(const v8::Arguments& args);
    static v8::Handle<v8::Value> SetJobId(const v8::Arguments& args);
    static v8::Handle<v8::Value> BeginPart(const v8::Arguments& args);
    static v8::Handle<v8::Value> SetEncoding(const v8::Arguments& args);
    static v8::Handle<v8::Value> AddRecipient(const v8::Arguments& args);
    static v8::Handle<v8::Value> AddMergeData(const v8::Arguments& args);
    static v8::Handle<v8::Value> SetReturnType(const v8::Arguments& args);
    static v8::Handle<v8::Value> SetEnvelopeId(const v8::Arguments& args);
    static v8::Handle<v8::Value> SetVirtualMta(const v8::Arguments& args);
    static v8::Handle<v8::Value> AddDateHeader(const v8::Arguments& args);

    const char* sender_;

  private:
    ~PMTAMessage();

};

class PMTARecipient : public node::ObjectWrap {
  
  public:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static void Init(v8::Handle<v8::Object> target);
    pmta::submitter::Recipient* recipient_;

  protected:
    PMTARecipient (const char* address);

    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    static v8::Handle<v8::Value> Address(const v8::Arguments& args);
    static v8::Handle<v8::Value> DefineVariable(const v8::Arguments& args);
    static v8::Handle<v8::Value> SetNotify(const v8::Arguments& args);

    const char* address_;

  private:
    ~PMTARecipient();
};

#endif
