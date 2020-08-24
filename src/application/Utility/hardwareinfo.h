#ifndef HARDWAREINFO_H
#define HARDWAREINFO_H

#include <QByteArray>
#include <QString>

class HardwareInfo
{
public:
    HardwareInfo();

    QString getCpuID() const;
    QString getBiosID() const;
    QString getDiskID() const;
    QString getMacAdd() const;

private:
    bool MakeHardwareInfoString(bool bThread = true);

private:
    QByteArray qbCpuID;
    QByteArray qbBiosID;
    QByteArray qbDiskID;
    QByteArray qbMacAdd;
    QByteArray qbOSVer;

};

#endif // HARDWAREINFO_H
