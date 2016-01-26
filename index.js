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
var pmta = null;

pmta = require('./build/Release/pmta');

exports.PmtaMsgRETURN_FULL      = "RETURN_FULL";
exports.PmtaMsgRETURN_HEADERS   = "RETURN_HEADERS";
exports.PmtaMsgENCODING_7BIT    = "ENCODING_7BIT";
exports.PmtaMsgENCODING_8BIT    = "ENCODING_8BIT";
exports.PmtaMsgENCODING_BASE64  = "ENCODING_BASE64";
exports.PmtaRcptNOTIFY_NEVER    = 0x00;
exports.PmtaRcptNOTIFY_SUCCESS  = 0x01;
exports.PmtaRcptNOTIFY_FAILURE  = 0x02;
exports.PmtaRcptNOTIFY_DELAY    = 0x04;
exports.Message                 = pmta.PMTAMessage;
exports.Recipient               = pmta.PMTARecipient;
exports.Connection              = pmta.PMTAConnection;
