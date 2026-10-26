#include "emailbanner.h"
#include "ui_emailbanner.h"

#include <QFontMetrics>
#include <QMouseEvent>
#include <QStyle>

EmailBanner::EmailBanner(QWidget *parent)
    : QWidget(parent), ui(new Ui::EmailBanner)
{
    ui->setupUi(this);
}

EmailBanner::EmailBanner(const Email &email, QWidget *parent)
    : EmailBanner(parent)
{
    setEmail(email);
}

EmailBanner::~EmailBanner() { delete ui; }

void EmailBanner::setEmail(const Email &email)
{
    m_messageId = email.messageId;
    m_sender    = email.senderUsername;
    m_subject   = email.subject.isEmpty() ? tr("(No subject)") : email.subject;

    // Collapse newlines/whitespace so the preview is a single line.
    m_preview = email.body.simplified();

    // Avatar: first letter + a colour derived from the sender ID.
    const QString initial = m_sender.isEmpty() ? "?" : m_sender.left(1).toUpper();
    ui->avatarLabel->setText(initial);
    const int hue = (email.senderId * 47) % 360;
    const QColor c = QColor::fromHsl(hue, 150, 110);
    ui->avatarLabel->setStyleSheet(
        QString("background-color: %1; color: white; border-radius: 20px;"
                "font-size: 16px; font-weight: bold;").arg(c.name()));

    // Recipients summary: "To: Alice +2"
    if (email.recipients.isEmpty()) {
        ui->recipientsLabel->clear();
    } else {
        QString text = "To: " + email.recipients.first().username;
        if (email.recipients.size() > 1)
            text += QString(" +%1").arg(email.recipients.size() - 1);
        ui->recipientsLabel->setText(text);
    }

    ui->senderLabel->setToolTip(m_sender);
    updateElidedText();
}

void EmailBanner::setSelected(bool selected)
{
    ui->bannerFrame->setProperty("selected", selected);
    ui->bannerFrame->style()->unpolish(ui->bannerFrame);
    ui->bannerFrame->style()->polish(ui->bannerFrame);
}

void EmailBanner::updateElidedText()
{
    auto elide = [](QLabel *label, const QString &text) {
        const int w = qMax(label->width(), 50);
        label->setText(label->fontMetrics().elidedText(text, Qt::ElideRight, w));
    };
    elide(ui->senderLabel,  m_sender);
    elide(ui->subjectLabel, m_subject);
    elide(ui->previewLabel, m_preview);
}

void EmailBanner::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateElidedText();
}

void EmailBanner::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked(m_messageId);
    QWidget::mousePressEvent(event);
}