import QtQuick 1.1
import com.nokia.meego 1.0
import "common"

Page {
    tools: commonTools

    function back() {
        webView.back.trigger();
    }

    function forward() {
        webView.forward.trigger();
    }




    // Header
    Header {
       id: header
       width: parent.width
       onUrlChanged: { webView.changeUrl(urlString); }
       loading: webView.loading
       z: 3
    }


    // WebView
    FlickableWebView {
       id: webView
       width: parent.width
       anchors.top: header.bottom
       //anchors.bottom: footer.top
       anchors.bottom: parent.bottom

       anchors.left: parent.left
       anchors.right: parent.right
       onGotFocus: { header.abortEdit(); }
       onUrlChanged: { header.setDefaultFavIcon(); header.changeUrl(urlString); }
       onIconChanged: { header.updateFavIcon(); }
       z: 1
    }

    // Attach scrollbars to the right and bottom edges of the view.
    ScrollBar {
       id: verticalScrollBar
       verticalOrientation: true
       whenToShow: webView.moving
       positionInContent: webView.visibleArea.yPosition
       visibleContent: webView.visibleArea.heightRatio
       width: 8
       anchors.top: parent.top
       anchors.topMargin: header.height
       anchors.right: parent.right
       anchors.bottom: parent.bottom
       anchors.bottomMargin: 8
       z: 3
    }

    // Attach scrollbars to the right and bottom edges of the view.
    ScrollBar {
       id: horizontalScrollBar
       verticalOrientation: false
       whenToShow: webView.moving
       positionInContent: webView.visibleArea.xPosition
       visibleContent: webView.visibleArea.widthRatio
       height: 8
       anchors.left: parent.left
       anchors.right: parent.right
       anchors.rightMargin: 8
       anchors.bottom: parent.bottom
       z: 3
    }

}
