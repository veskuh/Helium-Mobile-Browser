#include "WebViewInterface.h"

// public:
WebViewInterface::WebViewInterface(
      const QGraphicsObject *root,
      const QString &webViewObjectName)
{
   m_webView = root->findChild<QObject *>(webViewObjectName);
}

// public::
QString WebViewInterface::title() { return m_webView->property("title").toString(); }

QUrl WebViewInterface::url() { return m_webView->property("url").toUrl(); }

QPixmap WebViewInterface::icon() { return m_webView->property("icon").value<QPixmap>(); }

bool WebViewInterface::renderingEnabled() { return m_webView->property("renderingEnabled").toBool(); }

