/****************************************************************************
**
** Copyright (c) 2009-2010, Jaco Naude
**
** This file is part of Qtilities which is released under the following
** licensing options.
**
** Option 1: Open Source
** Under this license Qtilities is free software: you can
** redistribute it and/or modify it under the terms of the GNU General
** Public License as published by the Free Software Foundation, either
** version 3 of the License, or (at your option) any later version.
**
** Qtilities is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Qtilities. If not, see http://www.gnu.org/licenses/.
**
** Option 2: Commercial
** Alternatively, this library is also released under a commercial license
** that allows the development of closed source proprietary applications
** without restrictions on licensing. For more information on this option,
** please see the project website's licensing page:
** http://www.qtilities.org/licensing.html
**
** If you are unsure which license is appropriate for your use, please
** contact support@qtilities.org.
**
****************************************************************************/

#ifndef LOGGER_GUI_H
#define LOGGER_GUI_H

#include "QtilitiesCoreGui_global.h"
#include "LoggerConfigWidget.h"
#include "WidgetLoggerEngine.h"
#include "WidgetLoggerEngineFrontend.h"

#include <Logger.h>
#include <LoggingConstants.h>

#include <QWidget>
#include <QDockWidget>
#include <QPointer>

namespace Qtilities {
    namespace CoreGui {
        using namespace Qtilities::Logging;
        using namespace Qtilities::Logging::Constants;

        /*!
        \class LoggerGui
        \brief A class providing static member functions to acccess and create GUIs related to the %Qtilities Logging library.

        This class allows the creation of log widgets through static functions. These widgets includes:
        - A configuration widget for the logger.
        - Log widget. When creating a log widget, you can specify which message types must be displayed in your widget. The function will automatically create a new widget logger engine for you and
        the widget which is returned will be ready to use.
        - Log dock widget. Same as log widget, except that it returns a dock widget with the log widget already inside it.

        Below is an example dock log widget. Note the ready to use Qtilities::CoreGui::SearchBoxWidget at the bottom of the log window.

        \image html log_dock_widget.jpg "Log Dock Widget"
        \image latex log_dock_widget.eps "Log Dock Widget" width=\textwidth
          */
        class QTILITIES_CORE_GUI_SHARED_EXPORT LoggerGui {
        public:
            LoggerGui() {}
            virtual ~LoggerGui() {}

            //! Creates a new logger widget which logs messages of the specified verbosity. The user must manage the widget instance.
            static QWidget* createLogWidget(const QString& window_title, bool is_active, Logger::MessageTypeFlags message_types = Logger::AllLogLevels) {
                // Create a new logger widget engine and add it to the logger.
                WidgetLoggerEngine* new_widget_engine = new WidgetLoggerEngine();

                // Install a formatting engine for the new logger engine
                AbstractFormattingEngine* formatting_engine = Log->formattingEngineReference(FORMATTING_ENGINE_DEFAULT);
                if (formatting_engine) {
                    new_widget_engine->installFormattingEngine(formatting_engine);
                } else {
                    delete new_widget_engine;
                    LOG_ERROR(QString(QObject::tr("Failed to create log widget engine. The specified formatting engine could not be found: %1")).arg(FORMATTING_ENGINE_RICH_TEXT));
                    return 0;
                }

                new_widget_engine->setObjectName(window_title);
                Log->attachLoggerEngine(new_widget_engine);
                new_widget_engine->setActive(is_active);
                new_widget_engine->setEnabledMessageTypes(message_types);
                return new_widget_engine->getWidget();
            }

            //! Creates a new logger dock widget which logs messages of the specified verbosity. This function is similar to createLogWidget() but it wraps the widget produced by createLogWidget() with a QDockWidget(). The user must manage the widget instance.
            static QDockWidget* createLogDockWidget(const QString& window_title, bool is_active, Logger::MessageTypeFlags message_types = Logger::AllLogLevels) {
                QDockWidget* log_dock_widget = new QDockWidget(window_title);
                QWidget* log_widget = createLogWidget(window_title,is_active,message_types);
                log_dock_widget->setWidget(log_widget);
                QObject::connect(log_widget,SIGNAL(destroyed()),log_dock_widget,SLOT(deleteLater()));
                return log_dock_widget;
            }

            //! Return a settings widget for the logger.
            static QWidget* createLoggerConfigWidget(bool applyButtonVisible = true) {
                LoggerConfigWidget* config_widget = new LoggerConfigWidget(applyButtonVisible);
                return config_widget;
            }
        };
    }
}

#endif // LOGGER_GUI_H
