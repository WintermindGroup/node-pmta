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
    "fname" : "jane"
  }
];

var host  = "127.0.0.1",
    vmta  = "default",
    port  = 2525,
    verp  = true,
    job   = "xxx000xxx";

var msg = new pmta.Message("noreply@domain.tld");
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
var res = cn.submitSync(msg);
console.log(res);
