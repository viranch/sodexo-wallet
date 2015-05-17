#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    currency = QLocale(QLocale::English, QLocale::India).currencySymbol();

    ui->amtEdit->setPrefix(currency+" ");

    wallet = new Wallet;
    connect(wallet, SIGNAL(walletUpdated(UintMap)), this, SLOT(on_wallet_updated(UintMap)));
    wallet->init();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_addRechargeBtn_clicked()
{
    ui->rechargeTable->insertRow(ui->rechargeTable->rowCount());
}

void Dialog::on_rechargeBtn_clicked()
{
    uint coupon, count;
    for(int i=0; i<ui->rechargeTable->rowCount(); i++) {
        coupon = ui->rechargeTable->item(i, 0)->text().toUInt();
        count = ui->rechargeTable->item(i, 1)->text().toUInt();
        wallet->recharge(coupon, count);
    }
    while(ui->rechargeTable->rowCount() > 0) {
        ui->rechargeTable->removeRow(0);
    }
}

void Dialog::on_wallet_updated(UintMap newInventory)
{
    ui->inventoryList->clear();
    QList<QTreeWidgetItem*> rows;
    uint sum = 0;
    foreach(uint coupon, newInventory.keys()) {
        uint count = newInventory[coupon];
        uint amt = coupon * count;
        sum += amt;
        QStringList values;
        values << currency + QString::number(coupon) << "x "+QString::number(count) << currency + QString::number(amt);
        rows << new QTreeWidgetItem(values);
    }

    ui->inventoryList->addTopLevelItems(rows);
    ui->inventoryTotalLabel->setText("Total = "+currency+QString::number(sum));
}

void Dialog::on_calcBtn_clicked()
{
    // input target
    uint amount = ui->amtEdit->cleanText().toUInt();

    // get inventory sorted by coupon amounts
    UintMap walletInventory = wallet->getInventory();
    redeem = UintMap(walletInventory);

    // skew from target if target too high
    uint calcAmt = qMin(Wallet::balance(redeem), amount);

    // calculate the coupon counts
    while(Wallet::balance(redeem) != calcAmt) {
        // check if all combination tried
        bool all_zero = true;
        foreach(uint count, redeem.values()) {
            if (count != 0) {
                all_zero = false;
                break;
            }
        }
        // if all combination tried, try a lower amount
        // (and reset the test inventory)
        if (all_zero) {
            calcAmt--;
            foreach(uint coupon, redeem.keys()) {
                redeem[coupon] = walletInventory[coupon];
            }
            continue;
        }

        // next combination:
        // decrease the coupon count by 1
        foreach(uint coupon, redeem.keys()) {
            if (redeem[coupon] == 0) {
                redeem[coupon] = walletInventory[coupon];
            } else {
                redeem[coupon]--;
                break;
            }
        }
    }

    // we got it
    ui->couponList->clear();
    QList<QTreeWidgetItem*> rows;
    foreach(uint coupon, redeem.keys()) {
        uint count = redeem[coupon];
        if (count == 0) {
            continue;
        }
        uint amt = coupon * count;
        QStringList values;
        values << currency + QString::number(coupon) << "x "+QString::number(count) << currency + QString::number(amt);
        rows << new QTreeWidgetItem(values);
    }

    if (calcAmt != amount) {
        QStringList values;
        values << "Cash" << "" << currency + QString::number(amount-calcAmt);
        rows << new QTreeWidgetItem(values);
    }

    ui->couponList->addTopLevelItems(rows);
}

void Dialog::on_redeemBtn_clicked()
{
    wallet->redeem(redeem);
    ui->couponList->clear();
}
