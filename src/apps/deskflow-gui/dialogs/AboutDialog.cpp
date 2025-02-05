/*
 * Deskflow -- mouse and keyboard sharing utility
 * Copyright (C) 2024 Chris Rizzitello <sithlord48@gmail.com>
 * Copyright (C) 2012 Symless Ltd.
 * Copyright (C) 2008 Volker Lanz (vl@fidra.de)
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AboutDialog.h"

#include "common/constants.h"
#include "gui/style_utils.h"

#include <QClipboard>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
  setWindowTitle(tr("About Deskflow"));

  auto copyIcon = QIcon::fromTheme(
      QIcon::ThemeIcon::EditCopy, deskflow::gui::isDarkMode() ? QIcon(s_darkCopy) : QIcon(s_lightCopy)
  );

  auto btnCopyVersion = new QPushButton(copyIcon, QString(), this);
  btnCopyVersion->setFlat(true);
  connect(btnCopyVersion, &QPushButton::clicked, this, &AboutDialog::copyVersionText);

  // Set up the displayed version number
  auto versionString = QString(kVersion);
  if (versionString.endsWith(QStringLiteral(".0"))) {
    versionString.chop(2);
  } else {
    versionString.append(QStringLiteral(" (%1)").arg(kVersionGitSha));
  }

  auto versionLayout = new QHBoxLayout();
  versionLayout->addWidget(new QLabel(tr("Version:")));
  versionLayout->addWidget(new QLabel(versionString, this));
  versionLayout->addWidget(btnCopyVersion);
  versionLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));

  auto lblLogo = new QLabel(this);
  lblLogo->setPixmap(deskflow::gui::isDarkMode() ? s_darkLogo : s_lightLogo);

  auto lblCopyright = new QLabel(kCopyright, this);

  auto boldFont = font();
  boldFont.setBold(true);

  auto lblDevsTitle = new QLabel(tr("Important developers"));
  lblDevsTitle->setFont(boldFont);

  auto lblDevsBody = new QLabel(QStringLiteral("%1\n").arg(s_awesomeDevs.join(", ")));
  lblDevsBody->setWordWrap(true);

  auto btnOk = new QPushButton(tr("Ok"), this);
  btnOk->setDefault(true);
  connect(btnOk, &QPushButton::clicked, this, [this] { close(); });

  auto mainLayout = new QVBoxLayout();
  mainLayout->addWidget(lblLogo);
  mainLayout->addLayout(versionLayout);
  mainLayout->addWidget(new QLabel(tr("Keyboard and mouse sharing application"), this));
  mainLayout->addWidget(lblCopyright);
  mainLayout->addWidget(lblDevsTitle);
  mainLayout->addWidget(lblDevsBody);
  mainLayout->addWidget(btnOk);

  setLayout(mainLayout);
  adjustSize();
  setFixedSize(size());
}

void AboutDialog::copyVersionText()
{
  QString infoString = QStringLiteral("Deskflow: %1 (%2)\nQt: %3\nSystem: %4")
                           .arg(kVersion, kVersionGitSha, qVersion(), QSysInfo::prettyProductName());
#ifdef Q_OS_LINUX
  infoString.append(QStringLiteral("\nSession: %1 (%2)")
                        .arg(qEnvironmentVariable("XDG_CURRENT_DESKTOP"), qEnvironmentVariable("XDG_SESSION_TYPE")));
#endif
  QGuiApplication::clipboard()->setText(infoString);
}
