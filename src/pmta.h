/*! \file pmta.h The whole of the node-pmta can be found here and in pmta.cpp
 *
 * \mainpage
 * 
 * \section synopsis Synopsis
 * This module provides a NodeJS wrapper for the PMTA API.
 *
 * \section toc Table of Contents
 * - \ref license "License"
 * - \ref credits "Credits"
 *
 * \section license License
 * <div class="license">
 * Copyright (C) 2015  Dan Nielsen <dnielsen@reachmail.com>
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
 * </div>
 *
 * \section credits Credits
 * <div class="license">
 * Portability between NodeJS versions provided by nan 
 *   [ https://www.npmjs.com/package/nan ]
 * </div>
 */
#ifndef PMTA_RECIPIENT_H
#define PMTA_RECIPIENT_H

#include <nan.h>
#include <node.h>
#include <string.h>

#include "submitter/Message.hxx"
#include "submitter/Recipient.hxx"
#include "submitter/Connection.hxx"

/*!
 * \addtogroup connection PMTA Connection
 * \brief Represents a connection to a PMTA host.
 */
class PMTAConnection : public Nan::ObjectWrap {

  public:
    static void Init (v8::Local<v8::Object> exports);
    pmta::submitter::Connection* mConnection;

    ~PMTAConnection (void);

  protected:
    /*!
     * \brief Creates a new connection to a PMTA host
     * \param pHost Connection hostname
     * \param pPort Connection port
     * \param pName User name (optional)
     * \param pPassword Password (optional);
     *
     * Objects derived from this class represent a connection to a PMTA
     * instance on a remote host or the local host. 
     */
    PMTAConnection (const char* pHost, int pPort, const char* pName = "",
      const char* pPassword = "");

    static void New (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Submits a message to the connection
     * \param pMessage A Message object
     *
     * This method submits the supplied Message object to a PMTA host.
     */
    static void submit (const Nan::FunctionCallbackInfo<v8::Value>& info);

    const char *mHost;
    int         mPort;
    const char *mName;
    const char *mPassword;

  private:
    static Nan::Persistent<v8::Function> constructor;
};

/*!
 *
 * \addtogroup message PMTA message
 * \brief Represents a PMTA message
 *
 * Objects derived from this class represent a single e-mail message.
 */
class PMTAMessage : public Nan::ObjectWrap {

  public:
    static void Init (v8::Local<v8::Object> exports);
    pmta::submitter::Message* mMessage;

    ~PMTAMessage(void);

  protected:
    /*!
     * \brief Create as a PMTA message
     * \param pSender The message sender, i.e. Envelope From
     */
    PMTAMessage (const char* pSender);

    static void New          (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Convenience function, simply returns the Envelope From for 
     *        this message.
     * \return Message envelope from
     */
    static void sender       (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Add a section of data to the message. This can be a much or as 
     *        little of the message as is currently available. Anything from
     *        a single header to the entire message can be added.
     * \param pData. The data string to add to message.
     */
    static void addData      (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Set VERP (Variable Envelope Return-Path) for a message. Turning
     *        this value on will modify the Return-Path of the message to be 
     *        a concatenation of the From field and the Return-Path field to 
     *        create a unique Return-Path. 
     * \param VERP boolean T/F
     */
    static void setVerp      (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Sets the PMTA job id for a message. The job id can be used to 
     *        track the message through its lifetime in PMTA.
     * \prama Job id as a string.
     */
    static void setJobId     (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Begins a simple message part. This can be used to create MIME
     *        parts when composing a message directly through the interface.
     */
    static void beginPart    (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Set the encoding for this message. Parameter should be one of 
     *        PmtaMsgRETURN_FULL, PmtaMsgRETURN_HEADERS, PmtaMsgENCODING_7BIT,
     *        or PmtaMsgENCODING_8BIT, PmtaMsgENCODING_BASE64
     * \param pEncoding Message encoding type
     */
    static void setEncoding  (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Adds a Recipient to the message.
     * \param pRecipient PMTA Recipient
     */
    static void addRecipient (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Add merge data to the message. Similar to addData, this can
     *        be some or all of a message but can include mail merge variables.
     *        Merge variables are delimited with [ and ].
     * \param pData. The data string to add to the message.
     */
    static void addMergeData (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Sets the return type for the message. Can be overridden by 
     *        per-recipient values. Parameter should be one of 
     *        PmtaRcptNOTIFY_NEVER, PmtaRcptNOTIFY_SUCCESS, 
     *        PmtaRcptNOTIFY_FAILURE, PmtaRctpNOTIFY_DELAY
     * \param pReturnType Message return type
     */
    static void setReturnType(const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Set the envelope id for the message
     * \param pEnvelopeId.
     */
    static void setEnvelopeId(const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*! 
     * \brief Set the Virtual MTA for this message.
     * \param pVirtualMta. The Virtual MTA name.
     */
    static void setVirtualMta(const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Adds the Date field to the message headers
     */
    static void addDateHeader(const Nan::FunctionCallbackInfo<v8::Value>& info);

    const char *mSender;

  private:
    static Nan::Persistent<v8::Function> constructor;
};

/*!
 *
 * \addtogroup recipient PMTA Recipient
 * \brief Represents a PMTA Recipient
 *
 * Objects derived from this class represent a single e-mail recipient.
 */
class PMTARecipient : public Nan::ObjectWrap {

  public:
    static void Init (v8::Local<v8::Object> exports);
    pmta::submitter::Recipient* mRecipient;

    ~PMTARecipient (void);

  protected:
    /*!
     * \brief Creates a PMTA Recipient
     * \param pRecipient. E-mail address for this recipient
     */
    PMTARecipient (const char* pAddress);

    static void New (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Convenience method, simply returns the e-mail address defined
     *        for this recipient.
     * \return Defined e-mail address.
     */
    static void address (const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Defines a merge variable for this recipient.
     * \param pVariableName Merge variable value.
     * \param pVariableValue Merge variable value.
     */
    static void defineVariable (
      const Nan::FunctionCallbackInfo<v8::Value>& info);

    /*!
     * \brief Sets the return type for the message. Parameter should be one of 
     *        PmtaRcptNOTIFY_NEVER, PmtaRcptNOTIFY_SUCCESS, 
     *        PmtaRcptNOTIFY_FAILURE, PmtaRctpNOTIFY_DELAY
     * \param pReturnType Recipient return type
     */
    static void setNotify (const Nan::FunctionCallbackInfo<v8::Value>& info);

    const char *mAddress;

  private:
    static Nan::Persistent<v8::Function> constructor; 
};

#endif
