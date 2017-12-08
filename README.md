# proxysrv
A SIP Proxy Server forked from OpenSBC, with a couple bug fixes and two added features:

* Round-robin routing of inbound calls to registered UAs
* On-busy forwarding of overflow calls when all UAs are busy to destinations corresponding to the original destination (SIP To)
