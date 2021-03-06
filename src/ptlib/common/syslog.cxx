/*
 * syslog.cxx
 *
 * System Logging class.
 *
 * Portable Tools Library
 *
 * Copyright (c) 2009 Equivalence Pty. Ltd.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Portable Windows Library.
 *
 * The Initial Developer of the Original Code is Equivalence Pty. Ltd.
 *
 * Contributor(s): ______________________________________.
 *
 * $Revision: 29525 $
 * $Author: rjongbloed $
 * $Date: 2013-04-17 19:04:16 -0500 (Wed, 17 Apr 2013) $
 */

#include <ptlib.h>

#ifdef __GNUC__
#pragma implementation "syslog.h"
#endif

#include <ptlib/syslog.h>
#include <ptlib/pprocess.h>


/////////////////////////////////////////////////////////////////////////////

static struct PSystemLogTargetGlobal
{
  PSystemLogTargetGlobal()
  {
    m_targetPointer = new PSystemLogToNowhere;
    m_targetAutoDelete = true;
  }
  ~PSystemLogTargetGlobal()
  {
    if (m_targetAutoDelete)
      delete m_targetPointer;
    m_targetPointer = NULL;
  }
  void Set(PSystemLogTarget * target, bool autoDelete)
  {
    m_targetMutex.Wait();

    PSystemLog::Level level = m_targetPointer->GetThresholdLevel();

    if (m_targetAutoDelete)
      delete m_targetPointer;

    if (target != NULL) {
      m_targetPointer = target;
      m_targetAutoDelete = autoDelete;
    }
    else {
      m_targetPointer = new PSystemLogToNowhere;
      m_targetAutoDelete = true;
    }

    m_targetPointer->SetThresholdLevel(level);

    m_targetMutex.Signal();
  }

  PMutex             m_targetMutex;
  PSystemLogTarget * m_targetPointer;
  bool               m_targetAutoDelete;
} g_SystemLogTarget;


/////////////////////////////////////////////////////////////////////////////

PSystemLog::PSystemLog(Level level)   ///< only messages at this level or higher will be logged
  : P_IOSTREAM(cout.rdbuf())
  , m_logLevel(level)
{ 
  m_buffer.m_log = this;
  init(&m_buffer);
}


PSystemLog::PSystemLog(const PSystemLog & other)
  : PObject(other)
  , P_IOSTREAM(cout.rdbuf()) 
{
}


PSystemLog & PSystemLog::operator=(const PSystemLog &)
{ 
  return *this; 
}


///////////////////////////////////////////////////////////////

PSystemLog::Buffer::Buffer()
{
  PMEMORY_IGNORE_ALLOCATIONS_FOR_SCOPE;
  char * newptr = m_string.GetPointer(32);
  setp(newptr, newptr + m_string.GetSize() - 1);
}


streambuf::int_type PSystemLog::Buffer::overflow(int_type c)
{
  if (pptr() >= epptr()) {
    PMEMORY_IGNORE_ALLOCATIONS_FOR_SCOPE;

    size_t ppos = pptr() - pbase();
    char * newptr = m_string.GetPointer(m_string.GetSize() + 32);
    setp(newptr, newptr + m_string.GetSize() - 1);
    pbump(ppos);
  }

  if (c != EOF) {
    *pptr() = (char)c;
    pbump(1);
  }

  return 0;
}


streambuf::int_type PSystemLog::Buffer::underflow()
{
  return EOF;
}


int PSystemLog::Buffer::sync()
{
  PSystemLog::Level logLevel = m_log->m_logLevel;

#if PTRACING
  if (m_log->width() > 0 && (PTrace::GetOptions()&PTrace::SystemLogStream) != 0) {
    // Trace system sets the ios stream width as the last thing it does before
    // doing a flush, which gets us here. SO now we can get a PTRACE looking
    // exactly like a PSYSTEMLOG of appropriate level.
    unsigned traceLevel = (unsigned)m_log->width() - 1;
    m_log->width(0);
    if (traceLevel >= PSystemLog::NumLogLevels)
      traceLevel = PSystemLog::NumLogLevels-1;
    logLevel = (Level)traceLevel;
  }
#endif

  // Make sure there is a trailing NULL at end of string
  overflow('\0');

  g_SystemLogTarget.m_targetMutex.Wait();
  if (g_SystemLogTarget.m_targetPointer != NULL)
    g_SystemLogTarget.m_targetPointer->Output(logLevel, m_string);
  g_SystemLogTarget.m_targetMutex.Signal();

  PMEMORY_IGNORE_ALLOCATIONS_FOR_SCOPE;

  m_string.SetSize(10);
  char * base = m_string.GetPointer();
  *base = '\0';
  setp(base, base + m_string.GetSize() - 1);
 
  return 0;
}


PSystemLogTarget & PSystemLog::GetTarget()
{
  return *PAssertNULL(g_SystemLogTarget.m_targetPointer);
}


void PSystemLog::SetTarget(PSystemLogTarget * target, bool autoDelete)
{
  g_SystemLogTarget.Set(target, autoDelete);
}


///////////////////////////////////////////////////////////////

PSystemLogTarget::PSystemLogTarget()
  : m_thresholdLevel(PSystemLog::Warning)
{
}


PSystemLogTarget::PSystemLogTarget(const PSystemLogTarget & other)
  : PObject(other)
{
}


PSystemLogTarget & PSystemLogTarget::operator=(const PSystemLogTarget &)
{ 
  return *this; 
}


void PSystemLogTarget::OutputToStream(ostream & stream, PSystemLog::Level level, const char * msg)
{
  if (level > m_thresholdLevel || !PProcess::IsInitialised())
    return;

  PTime now;
  stream << now.AsString("yyyy/MM/dd hh:mm:ss.uuu\t");

  if (level < 0)
    stream << "Message";
  else {
    static const char * const levelName[] = {
      "Fatal error",
      "Error",
      "Warning",
      "Info"
    };
    if (level < PARRAYSIZE(levelName))
      stream << levelName[level];
    else
      stream << "Debug" << (level - PSystemLog::Info);
  }

  stream << '\t' << msg;
  if (msg[0] == '\0' || msg[strlen(msg)-1] != '\n')
    stream << endl;
}


///////////////////////////////////////////////////////////////

void PSystemLogToStderr::Output(PSystemLog::Level level, const char * msg)
{
  OutputToStream(cerr, level, msg);
}


///////////////////////////////////////////////////////////////

#if PTRACING
PSystemLogToTrace::PSystemLogToTrace()
{
  // Must be off or recurses!
  PTrace::ClearOptions(PTrace::SystemLogStream);
  if (PIsDescendant(PTrace::GetStream(), PSystemLog))
    PTrace::SetStream(NULL);
}

void PSystemLogToTrace::Output(PSystemLog::Level level, const char * msg)
{
  if (PTrace::CanTrace(level))
    PTrace::Begin(level, NULL, 0) << msg << PTrace::End;
}
#endif


///////////////////////////////////////////////////////////////

PSystemLogToFile::PSystemLogToFile(const PString & filename)
  : m_file(filename, PFile::WriteOnly)
{
}


void PSystemLogToFile::Output(PSystemLog::Level level, const char * msg)
{
  OutputToStream(m_file, level, msg);
}


///////////////////////////////////////////////////////////////

PSystemLogToNetwork::PSystemLogToNetwork(const PIPSocket::Address & address, WORD port, unsigned facility)
  : m_server(address, port)
  , m_facility(facility)
{
}


PSystemLogToNetwork::PSystemLogToNetwork(const PString & server, WORD port, unsigned facility)
  : m_facility(facility)
{
  m_server.Parse(server, port, ':', "udp");
}


void PSystemLogToNetwork::Output(PSystemLog::Level level, const char * msg)
{
  if (level > m_thresholdLevel || !m_server.IsValid() || !PProcess::IsInitialised())
    return;

  static int PwlibLogToSeverity[PSystemLog::NumLogLevels] = {
    2, 3, 4, 5, 6, 7, 7, 7, 7, 7
  };

  PStringStream str;
  str << '<' << (((m_facility*8)+PwlibLogToSeverity[level])%1000) << '>'
    << PTime().AsString("MMM dd hh:mm:ss ")
    << PIPSocket::GetHostName() << ' '
    << PProcess::Current().GetName() << ' '
    << msg;
  m_socket.WriteTo((const char *)str, str.GetLength(), m_server);
}


///////////////////////////////////////////////////////////////

#ifdef WIN32
void PSystemLogToDebug::Output(PSystemLog::Level level, const char * msg)
{
  if (level > m_thresholdLevel || !PProcess::IsInitialised())
    return;

  PStringStream strm;
  OutputToStream(strm, level, msg);
  PVarString str = strm;
  OutputDebugString(str);
}
#else

#include <syslog.h>

PSystemLogToSyslog::PSystemLogToSyslog(const char * ident, int priority, int options, int facility)
  : m_ident(ident)
  , m_priority(priority)
{
  if (m_ident.IsEmpty())
    m_ident = PProcess::Current().GetName();

  if (options < 0)
    options = LOG_PID;

  if (facility < 0)
    facility = LOG_DAEMON;

  openlog(m_ident, options, facility);
}


PSystemLogToSyslog::~PSystemLogToSyslog()
{
  closelog();
}


void PSystemLogToSyslog::Output(PSystemLog::Level level, const char * msg)
{
  if (level > m_thresholdLevel || !PProcess::IsInitialised())
    return;

  int priority = m_priority;
  if (priority < 0) {
    switch (level) {
      case PSystemLog::Fatal :
        priority = LOG_CRIT;
        break;
      case PSystemLog::Error :
        priority = LOG_ERR;
        break;
      case PSystemLog::StdError :
      case PSystemLog::Warning :
        priority = LOG_WARNING;
        break;
      case PSystemLog::Info :
        priority = LOG_INFO;
        break;
      default :
        priority = LOG_DEBUG;
    }
    syslog(priority, "%s", msg);
  }
  else {
    static const char * const levelName[] = {
        "FATAL",    // LogFatal,
        "ERROR",    // LogError,
        "WARNING",  // LogWarning,
        "INFO",     // LogInfo,
    };
    if (level < PARRAYSIZE(levelName))
      syslog(priority, "%-8s%s", levelName[level], msg);
    else
      syslog(priority, "DEBUG%-3u%s", level - PSystemLog::Info, msg);
  }
}
#endif


// End Of File ///////////////////////////////////////////////////////////////
