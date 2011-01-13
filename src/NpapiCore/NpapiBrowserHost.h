/**********************************************************\
Original Author: Richard Bateman (taxilian)

Created:    Oct 15, 2009
License:    Dual license model; choose one of two:
            New BSD License
            http://www.opensource.org/licenses/bsd-license.php
            - or -
            GNU Lesser General Public License, version 2.1
            http://www.gnu.org/licenses/lgpl-2.1.html

Copyright 2009 Richard Bateman, Firebreath development team
\**********************************************************/

#pragma once
#ifndef H_NPAPIBROWSERHOST
#define H_NPAPIBROWSERHOST

#include "NpapiTypes.h"
#include "BrowserHost.h"
#include <boost/thread.hpp>

namespace FB { namespace Npapi {

    class NpapiPluginModule;
    class NPObjectAPI;
    typedef boost::shared_ptr<NPObjectAPI> NPObjectAPIPtr;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @class  NpapiBrowserHost
    ///
    /// @brief  Provides a FB::BrowserHost implementation for Npapi
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    class NpapiBrowserHost :
        public FB::BrowserHost
    {
    public:
        NpapiBrowserHost(NpapiPluginModule *module, NPP npp);
        virtual ~NpapiBrowserHost(void);

        void setBrowserFuncs(NPNetscapeFuncs *pFuncs);

    public:
        virtual BrowserStreamPtr createStream(const std::string& url, const PluginEventSinkPtr& callback, 
                                            bool cache = true, bool seekable = false, 
                                            size_t internalBufferSize = 128 * 1024 ) const;

    public:
        virtual void ScheduleAsyncCall(void (*func)(void *), void *userData) const;
        virtual void *getContextID() const { return (void *)m_npp; }

    public:
        FB::DOM::DocumentPtr getDOMDocument();
        FB::DOM::WindowPtr getDOMWindow();
        FB::DOM::ElementPtr getDOMElement();
        void evaluateJavaScript(const std::string &script);
        bool isSafari() const;

    public:
        FB::variant getVariant(const NPVariant *npVar);
        void getNPVariant(NPVariant *dst, const FB::variant &var);

    // NPN_ functions -- for scope reasons, we no longer access these using the global functions
    protected:
        NPNetscapeFuncs NPNFuncs;   // Function pointers
        NpapiPluginModule *module;
        NPP m_npp;
        NPObjectAPIPtr m_htmlDoc;
        NPObjectAPIPtr m_htmlWin;
        NPObjectAPIPtr m_htmlElement;

    public:
        void* MemAlloc(uint32_t size) const;
        void MemFree(void* ptr) const;
        uint32_t MemFlush(uint32_t size) const;

        NPObject *RetainObject(NPObject *npobj) const;
        void ReleaseObject(NPObject *npobj) const;
        void ReleaseVariantValue(NPVariant *variant) const;

        NPIdentifier GetStringIdentifier(const NPUTF8 *name) const;
        void GetStringIdentifiers(const NPUTF8 **names, int32_t nameCount, NPIdentifier *identifiers) const;
        NPIdentifier GetIntIdentifier(int32_t intid) const;
        bool IdentifierIsString(NPIdentifier identifier) const;
        NPUTF8 *UTF8FromIdentifier(NPIdentifier identifier) const;
        std::string StringFromIdentifier(NPIdentifier identifier) const;
        int32_t IntFromIdentifier(NPIdentifier identifier) const;

        /* npapi.h definitions */
        NPError GetURLNotify(const char* url, const char* target, void* notifyData) const;
        NPError GetURL(const char* url, const char* target) const;
        NPError PostURLNotify(const char* url, const char* target, uint32_t len, const char* buf, NPBool file, void* notifyData) const;
        NPError PostURL(const char* url, const char* target, uint32_t len, const char* buf, NPBool file) const;
        NPError RequestRead(NPStream* stream, NPByteRange* rangeList) const;
        NPError NewStream(NPMIMEType type, const char* target, NPStream** stream) const;
        int32_t Write(NPStream* stream, int32_t len, void* buffer) const;
        NPError DestroyStream(NPStream* stream, NPReason reason) const;
        void SetStatus(const char* message) const;
        const char* UserAgent() const;
        NPError GetValue(NPNVariable variable, void *value) const;
        NPError SetValue(NPPVariable variable, void *value) const;
        void InvalidateRect(NPRect *invalidRect) const;
        void InvalidateRegion(NPRegion invalidRegion) const;
        void ForceRedraw() const;
        void PushPopupsEnabledState(NPBool enabled) const;
        void PopPopupsEnabledState() const;
        void PluginThreadAsyncCall(void (*func) (void *), void *userData) const;
        /* npruntime.h definitions */
        NPObject *CreateObject(NPClass *aClass) const;
        bool Invoke(NPObject *npobj, NPIdentifier methodName, const NPVariant *args,
                                        uint32_t argCount, NPVariant *result) const;
        bool InvokeDefault(NPObject *npobj, const NPVariant *args,
                                        uint32_t argCount, NPVariant *result) const;
        bool Evaluate(NPObject *npobj, NPString *script,
                                        NPVariant *result) const;
        bool GetProperty(NPObject *npobj, NPIdentifier propertyName,
                                        NPVariant *result) const;
        bool SetProperty(NPObject *npobj, NPIdentifier propertyName,
                                        const NPVariant *value) const;
        bool RemoveProperty(NPObject *npobj, NPIdentifier propertyName) const;
        bool HasProperty(NPObject *npobj, NPIdentifier propertyName) const;
        bool HasMethod(NPObject *npobj, NPIdentifier methodName) const;
        bool Enumerate(NPObject *npobj, NPIdentifier **identifier,
                                        uint32_t *count) const;
        bool Construct(NPObject *npobj, const NPVariant *args,
                                        uint32_t argCount, NPVariant *result) const;
        void SetException(NPObject *npobj, const NPUTF8 *message) const;

        int ScheduleTimer(int interval, bool repeat, void(*func)(NPP npp, uint32_t timerID)) const;
        void UnscheduleTimer(int timerId) const;
    };

    typedef boost::shared_ptr<NpapiBrowserHost> NpapiBrowserHostPtr;
    typedef boost::shared_ptr<const NpapiBrowserHost> NpapiBrowserHostConstPtr;
    typedef boost::weak_ptr<NpapiBrowserHost> NpapiBrowserHostWeakPtr;
    typedef boost::weak_ptr<const NpapiBrowserHost> NpapiBrowserHostWeakConstPtr;
}; };

#endif
