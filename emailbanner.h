#pragma once

#include <QWidget>
#include "email.h"   // wherever your Email struct lives

namespace Ui { class EmailBanner; }

class EmailBanner : public QWidget {
    Q_OBJECT
public:
    explicit EmailBanner(QWidget *parent = nullptr);
    explicit EmailBanner(const Email &email, QWidget *parent = nullptr);
    ~EmailBanner() override;

    // The callable: fills the banner from an Email.
    void setEmail(const Email &email);
    int  messageId() const { return m_messageId; }
    void setSelected(bool selected);

signals:
    void clicked(int messageId);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateElidedText();

    Ui::EmailBanner *ui;
    int     m_messageId = -1;
    QString m_subject;
    QString m_preview;
    QString m_sender;
};