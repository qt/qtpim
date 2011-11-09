/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QVERSITDEFS_P_H
#define QVERSITDEFS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qversitglobal.h>
#include <QString>

QTVERSIT_BEGIN_NAMESPACE

// Mapping between a string in versit specifications and Qt contacts
struct VersitMapping {
    const char* versitString;
    const QString contactString;
};

// Mapping between a string in versit specifications and Qt contact details
struct VersitDetailMapping {
    const char* versitPropertyName;
    const QString detailDefinitionName;
    const QString detailFieldName;
};

//! [File extension mappings]
// Mappings from mime types to file extensions
const VersitMapping versitFileExtensionMappings[] = {
    {"application/octet-stream", QStringLiteral("obj")},
    {"audio/x-pn-realaudio", QStringLiteral("ra")},
    {"application/xml", QStringLiteral("wsdl")},
    {"application/octet-stream", QStringLiteral("dll")},
    {"image/x-cmu-raster", QStringLiteral("ras")},
    {"application/x-pn-realaudio", QStringLiteral("ram")},
    {"application/x-bcpio", QStringLiteral("bcpio")},
    {"application/x-sh", QStringLiteral("sh")},
    {"video/mpeg", QStringLiteral("m1v")},
    {"image/x-xwindowdump", QStringLiteral("xwd")},
    {"video/x-msvideo", QStringLiteral("avi")},
    {"image/x-ms-bmp", QStringLiteral("bmp")},
    {"application/x-shar", QStringLiteral("shar")},
    {"application/x-javascript", QStringLiteral("js")},
    {"application/x-wais-source", QStringLiteral("src")},
    {"application/x-dvi", QStringLiteral("dvi")},
    {"audio/x-aiff", QStringLiteral("aif")},
    {"text/plain", QStringLiteral("ksh")},
    {"application/msword", QStringLiteral("dot")},
    {"message/rfc822", QStringLiteral("mht")},
    {"application/x-pkcs12", QStringLiteral("p12")},
    {"text/css", QStringLiteral("css")},
    {"application/x-csh", QStringLiteral("csh")},
    {"application/vnd.ms-powerpoint", QStringLiteral("pwz")},
    {"application/pdf", QStringLiteral("pdf")},
    {"application/x-netcdf", QStringLiteral("cdf")},
    {"text/plain", QStringLiteral("pl")},
    {"text/plain", QStringLiteral("c")},
    {"image/jpeg", QStringLiteral("jpe")},
    {"image/jpeg", QStringLiteral("jpg")},
    {"text/x-python", QStringLiteral("py")},
    {"text/xml", QStringLiteral("xml")},
    {"image/jpeg", QStringLiteral("jpeg")},
    {"application/postscript", QStringLiteral("ps")},
    {"application/x-gtar", QStringLiteral("gtar")},
    {"image/x-xpixmap", QStringLiteral("xpm")},
    {"application/x-hdf", QStringLiteral("hdf")},
    {"message/rfc822", QStringLiteral("nws")},
    {"text/tab-separated-values", QStringLiteral("tsv")},
    {"application/xml", QStringLiteral("xpdl")},
    {"application/pkcs7-mime", QStringLiteral("p7c")},
    {"application/postscript", QStringLiteral("eps")},
    {"image/ief", QStringLiteral("ief")},
    {"application/octet-stream", QStringLiteral("so")},
    {"application/vnd.ms-excel", QStringLiteral("xlb")},
    {"image/x-portable-bitmap", QStringLiteral("pbm")},
    {"application/x-texinfo", QStringLiteral("texinfo")},
    {"application/vnd.ms-excel", QStringLiteral("xls")},
    {"application/x-tex", QStringLiteral("tex")},
    {"text/richtext", QStringLiteral("rtx")},
    {"text/html", QStringLiteral("html")},
    {"audio/x-aiff", QStringLiteral("aiff")},
    {"audio/x-aiff", QStringLiteral("aifc")},
    {"application/octet-stream", QStringLiteral("exe")},
    {"text/x-sgml", QStringLiteral("sgm")},
    {"image/tiff", QStringLiteral("tif")},
    {"video/mpeg", QStringLiteral("mpeg")},
    {"application/x-ustar", QStringLiteral("ustar")},
    {"image/gif", QStringLiteral("gif")},
    {"application/vnd.ms-powerpoint", QStringLiteral("ppt")},
    {"application/vnd.ms-powerpoint", QStringLiteral("pps")},
    {"text/x-sgml", QStringLiteral("sgml")},
    {"image/x-portable-pixmap", QStringLiteral("ppm")},
    {"application/x-latex", QStringLiteral("latex")},
    {"text/plain", QStringLiteral("bat")},
    {"video/quicktime", QStringLiteral("mov")},
    {"application/vnd.ms-powerpoint", QStringLiteral("ppa")},
    {"application/x-troff", QStringLiteral("tr")},
    {"application/xml", QStringLiteral("rdf")},
    {"application/xml", QStringLiteral("xsl")},
    {"message/rfc822", QStringLiteral("eml")},
    {"application/x-netcdf", QStringLiteral("nc")},
    {"application/x-sv4cpio", QStringLiteral("sv4cpio")},
    {"application/octet-stream", QStringLiteral("bin")},
    {"text/plain", QStringLiteral("h")},
    {"application/x-tcl", QStringLiteral("tcl")},
    {"application/msword", QStringLiteral("wiz")},
    {"application/octet-stream", QStringLiteral("o")},
    {"application/octet-stream", QStringLiteral("a")},
    {"application/postscript", QStringLiteral("ai")},
    {"audio/x-wav", QStringLiteral("wav")},
    {"text/x-vcard", QStringLiteral("vcf")},
    {"image/x-xbitmap", QStringLiteral("xbm")},
    {"text/plain", QStringLiteral("txt")},
    {"audio/basic", QStringLiteral("au")},
    {"application/x-troff", QStringLiteral("t")},
    {"image/tiff", QStringLiteral("tiff")},
    {"application/x-texinfo", QStringLiteral("texi")},
    {"application/oda", QStringLiteral("oda")},
    {"application/x-troff-ms", QStringLiteral("ms")},
    {"image/x-rgb", QStringLiteral("rgb")},
    {"application/x-troff-me", QStringLiteral("me")},
    {"application/x-sv4crc", QStringLiteral("sv4crc")},
    {"video/quicktime", QStringLiteral("qt")},
    {"video/mpeg", QStringLiteral("mpa")},
    {"video/mpeg", QStringLiteral("mpg")},
    {"video/mpeg", QStringLiteral("mpe")},
    {"application/msword", QStringLiteral("doc")},
    {"image/x-portable-graymap", QStringLiteral("pgm")},
    {"application/vnd.ms-powerpoint", QStringLiteral("pot")},
    {"application/x-mif", QStringLiteral("mif")},
    {"application/x-troff", QStringLiteral("roff")},
    {"text/html", QStringLiteral("htm")},
    {"application/x-troff-man", QStringLiteral("man")},
    {"text/x-setext", QStringLiteral("etx")},
    {"application/zip", QStringLiteral("zip")},
    {"video/x-sgi-movie", QStringLiteral("movie")},
    {"application/x-python-code", QStringLiteral("pyc")},
    {"image/png", QStringLiteral("png")},
    {"application/x-pkcs12", QStringLiteral("pfx")},
    {"message/rfc822", QStringLiteral("mhtml")},
    {"application/x-tar", QStringLiteral("tar")},
    {"image/x-portable-anymap", QStringLiteral("pnm")},
    {"application/x-python-code", QStringLiteral("pyo")},
    {"audio/basic", QStringLiteral("snd")},
    {"application/x-cpio", QStringLiteral("cpio")},
    {"application/x-shockwave-flash", QStringLiteral("swf")},
    {"audio/mpeg", QStringLiteral("mp3")},
    {"audio/mpeg", QStringLiteral("mp2")}
};
//! [File extension mappings]

QTVERSIT_END_NAMESPACE

#endif // QVERSITDEFS_P_H
