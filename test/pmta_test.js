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
 *
 * Using this test script
 *
 * Before using this script make sure to change the values defined in the 
 * `pmta options` and `message options` sections to values that will work
 * with your PMTA installation. Also, make sure that your PMTA installation
 * is configured to accept API submissions from whatever host this script will
 * be run on. See the PMTA documentation for the needed configuration 
 * directives.
 * 
 */
var pmta = require('../index.js');

// PMTA options. Change as needed to reflect your installation
var pmta_host = "10.0.0.47";
var pmta_port = 25;

// Message options. Change as needed.
var vmta      = "default";
var sender    = "noreply@domain.tld";
var verp      = true;
var job       = "00000999";

var recipients = [
  {
    "to"    : "jane@domain.tld",
    "fname" : "jane"
  }
];

var payload = [
  "From: [*from]",
  "To: <[*to]>",
  "Subject: PMTA message",
  "MIME-Version: 1.0",
  "Content-Type: text/plain; charset=utf-8",
  "Content-Transfer-Encoding: 7bit",
  "\n",
  "This is a message, [fname]\n"
].join("\n");

// End options. No need to edit beyond this line.

var msg = new pmta.Message(sender);

for (var j=0; j < recipients.length; j++) {
  var rcpt = new pmta.Recipient(recipients[j].to);
  rcpt.defineVariable("*parts", "1");
  rcpt.defineVariable("to", recipients[j].to);
  rcpt.defineVariable("fname", recipients[j].fname);
  msg.addRecipient(rcpt);
}

msg.addDateHeader();
msg.addMergeData(payload, payload.length);
msg.setEncoding(pmta.PmtaMsgENCODING_7BIT);
msg.setVirtualMta(vmta);
msg.setJobId(job);
msg.setVerp(verp);

var cn = new pmta.Connection(pmta_host, pmta_port);
var res = cn.submit(msg);
console.log(res);
