### Overview
node-pmta provides a NodeJS module ('pmta') for creating and submitting 
messages to [pmta](http://www.port25.com/powermta/powermta-overview/overview/).

The `pmta` module includes all the classes and methods detailed in the pmta
C++ submission API.

### Prerequisites
* [NodeJS v0.10+](http://www.nodejs.org)
* npm - Usually packaged with NodeJS
* [pmta](http://www.port25.com) submission API.

### Installation
Installation is still manually managed at the moment. Additional convenience
will be added soon.

    cd /path/to/myapp/
    mkdir pmta
    cd pmta
    git clone git@github.com:danielsen/node-pmta.git
    npm build .

### Usage
Objects and methods in the module mirror those in the pmta C++ submission API.

    var pmta = require('../index.js');

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

    var recipients = [
      {
        "to"    : "jane@domain.tld",
        "fname" : "Jane"
      }
    ];

    var host  = "127.0.0.1",
        vmta  = "default",
        port  = 2525,
        verp  = true,
        job   = "xxx000xxx";

    var msg = new pmta.Message("sender@domain.tld");
    var rcpt = new pmta.Recipient("jane@domain.tld");
    rcpt.defineVariable("to", "jane@domain.tld");

    for (var j=0; j < recipients.length; j++) {
      var rcpt = new pmta.Recipient(recipients[j].to);
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

    var cn = new pmta.Connection(host, port);
    var res = cn.submit(msg);
    console.log(res);
