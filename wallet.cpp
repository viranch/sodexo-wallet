#include "wallet.h"
#include <QSettings>

Wallet::Wallet(QObject *parent) :
    QObject(parent)
{
    connect(this, SIGNAL(walletUpdated(UintMap)),
            this, SLOT(dumpInventory()));
}

void Wallet::init()
{
    readInventory();
}

void Wallet::readInventory()
{
    QSettings s;
    QVariant value = s.value("inventory");
    if (value.isNull()) {
        return;
    }

    QMap<QString, QVariant> read = value.toMap();
    uint ukey, uval;
    bool ok;
    inventory.clear();
    foreach(QString key, read.keys()) {
        ukey = key.toUInt(&ok);
        if (!ok) continue;
        uval = read[key].toUInt(&ok);
        if (!ok) continue;
        inventory[ukey] = uval;
    }
    emit walletUpdated(inventory);
}

void Wallet::recharge(uint unit, uint count)
{
    if(!inventory.contains(unit)) {
        inventory[unit] = 0;
    }
    inventory[unit] += count;

    emit walletUpdated(inventory);
}

void Wallet::redeem(UintMap cash)
{
    foreach(uint unit, cash.keys()) {
        inventory[unit] -= cash[unit];
        if (inventory[unit] == 0) {
            inventory.remove(unit);
        }
    }
    emit walletUpdated(inventory);
}

void Wallet::dumpInventory()
{
    QSettings s;
    QMap<QString, QVariant> save;
    foreach(uint key, inventory.keys()) {
        save[QString::number(key)] = inventory[key];
    }

    s.setValue("inventory", save);
}

UintMap Wallet::getInventory()
{
    return inventory;
}

uint Wallet::balance(UintMap cash)
{
    uint bal = 0;
    foreach(uint amt, cash.keys()) {
        bal += amt * cash[amt];
    }
    return bal;
}
