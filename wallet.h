#ifndef WALLET_H
#define WALLET_H

#include <QObject>
#include <QMap>

typedef QMap<uint,uint> UintMap;

class Wallet : public QObject
{
    Q_OBJECT
public:
    explicit Wallet(QObject *parent = 0);

    void init();
    void recharge(uint unit, uint count);
    void redeem(UintMap cash);
    UintMap getInventory();
    static uint balance(UintMap cash);

signals:
    void walletUpdated(UintMap newInventory);

public slots:

private slots:
    void dumpInventory();

private:
    void readInventory();
    UintMap inventory;

};

#endif // WALLET_H
