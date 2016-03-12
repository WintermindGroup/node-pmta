node-pmta provides a NodeJS module ('pmta') for creating and submitting 
messages to [pmta](http://www.port25.com/powermta/powermta-overview/overview/).

### Overview
The `pmta` module includes all the classes and methods detailed in the Pmta
C++ submission API. For more information on these methods consult the Pmta
user guide or run doxygen to build local documentation.

### Prerequisites
* [NodeJS v0.10+](http://www.nodejs.org)
* npm - Usually packaged with NodeJS
* [pmta](http://www.port25.com) submission API.

### Installation
    npm install danielsen/node-pmta

or

    npm install pmta

or

    cd /path/to/myapp/
    mkdir pmta
    cd pmta
    git clone git@github.com:danielsen/node-pmta.git
    npm install

### Testing
After installation is complete, change to the `test` directory and run the
test script after modifying any of the variables specific to your Pmta 
installation, i.e. host, port, etc.

    cd test
    node pmta_test

### Documentation
The best documentation is the Pmta user guide. This module implements all the
methods described in the user guide. Local documentation can also be generated
using the doxygen command.

To generate the local documentation run

    doxygen Doxyfile

in the main application directory.

### Usage
Objects and methods in the module mirror those in the pmta C++ submission API.

    var pmta = require('./index.js');

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
        job   = "000000999";

    var msg = new pmta.Message("noreply@domain.tld");

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

    var pmta_connection   = new pmta.Connection(host, port);
    var submission_result = pmta_connection.submit(msg);
    console.log(submission_result);
