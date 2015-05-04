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
    QMap<QString, QVariant> read = s.value("inventory").toMap();
    inventory.clear();
    foreach(QString key, read.keys()) {
        inventory[key.toUInt()] = read[key].toUInt();
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
