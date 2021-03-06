/*
 * icmpsock.h
 *
 * Internet Control Message Protocol socket I/O channel class.
 *
 * Portable Windows Library
 *
 * Copyright (c) 1993-1998 Equivalence Pty. Ltd.
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
 * Portions are Copyright (C) 1993 Free Software Foundation, Inc.
 * All Rights Reserved.
 *
 * Contributor(s): ______________________________________.
 *
 * $Revision: 28484 $
 * $Author: rjongbloed $
 * $Date: 2012-10-05 03:53:11 -0500 (Fri, 05 Oct 2012) $
 */

///////////////////////////////////////////////////////////////////////////////
// PICMPSocket

  protected:
    PBoolean WritePing(
      const PString & host,   // Host to send ping.
      PingInfo & info         // Information on the ping and reply.
    );
    /* Send an ECHO_REPLY message to the specified host.

       <H2>Returns:</H2>
       false if host not found or no response.
     */

    PBoolean ReadPing(
      PingInfo & info         // Information on the ping and reply.
    );
    /* Receive an ECHO_REPLY message from the host.

       <H2>Returns:</H2>
       false if an error occurred.
     */

// End Of File ////////////////////////////////////////////////////////////////
