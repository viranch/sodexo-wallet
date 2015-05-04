#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "wallet.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_addRechargeBtn_clicked();

    void on_rechargeBtn_clicked();

    void on_wallet_updated(UintMap newInventory);

    void on_calcBtn_clicked();

    void on_redeemBtn_clicked();

private:
    Ui::Dialog *ui;
    Wallet *wallet;
    QString currency;
    UintMap redeem;
};

#endif // DIALOG_H
