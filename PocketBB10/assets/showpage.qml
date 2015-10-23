import bb.cascades 1.0
import bb.system 1.0
import custom 1.0
import "globalvar.js" as Gvar
Page {
objectName: "showpage"
    property alias myWebView : myWebView
    property alias progressBar : progressBar
    property alias offlineUrl : offlineUrl
    property alias currentID : currentID 
    property alias onlineUrl: onlineUrl
  //  property alias sqldata: sqldata    
    Container {
        layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
        Label {
            id: offlineUrl;
            visible: false;
        }
        Label {
            id: onlineUrl;
            visible: false;
        }
        Label { //da rivedere... non è possibile sia cosi'.....
            id: currentID
            visible: false;
        }
        ProgressIndicator {
            objectName: "progressBar"
            id: progressBar
            fromValue: 0
            toValue: 100
            minWidth: 768
        }
        ScrollView {
             scrollViewProperties {
                    scrollMode: ScrollMode.Both
                    pinchToZoomEnabled: true
                    maxContentScale: 5
                    minContentScale: 1 
                }
            WebView {
                url: "http://www.google.com"
                id: myWebView
                objectName: "myWebView"
                onLoadProgressChanged: { progressBar.value = loadProgress;}  
                onLoadingChanged: {
                    if (loadRequest.status == WebLoadStatus.Started) {
                        progressBar.visible = true;
                    }
                    else if (loadRequest.status == WebLoadStatus.Succeeded) {
                        progressBar.visible = false;
                    }
                    else if (loadRequest.status == WebLoadStatus.Failed) {
                        myWebView.url = "local:///assets/web/index.html";
                        progressBar.visible = false;
                    }
                }
                onMinContentScaleChanged: {
                               scrollView.scrollViewProperties.minContentScale = minContentScale;
                }
                onMaxContentScaleChanged: {
                               scrollView.scrollViewProperties.maxContentScale = maxContentScale;
                }
            }
        }attachedObjects: [
                    SystemToast {
                       id: myQmlDeleteById
                       body: "Deleted!"
                   },
                   SystemToast {
                       id: myQmlCheck
                       body: "Checked!"
                   },
                   SystemToast {
                       id: myQmlFav
                       body: "Now, it's in your Fav-List!"
                   },
                   SystemToast {
                      id: myQmlPaste
                      body: "Link copied."
                  }]
   
    }  
    actions: [
                ActionItem {
                    id: archiveItem
                    objectName: "archiveItem"
                    title: "Archive It"
                    imageSource: "asset:///images/checkmark.png"
                    ActionBar.placement: ActionBarPlacement.OnBar
                    onTriggered: { archive(); }
                    shortcuts: [
                    	Shortcut {
                    		key: qsTr("A")
                    		onTriggered: { archive(); }
                    }]
            	},
                ActionItem {
                    id: copyLinkItem
                    objectName: "copyLinkItem"
                    title: "Copy Link"
                    imageSource: "asset:///images/ic_copy_link.png"
                    ActionBar.placement: ActionBarPlacement.InOverflow
                    onTriggered: { copyLink(); }
                    shortcuts: [
                    	Shortcut {
                    		key: qsTr("C")
                    		onTriggered: { copyLink(); }
                    }]
                },
                InvokeActionItem {
                         title: "Share"
                         ActionBar.placement: ActionBarPlacement.OnBar
                         query {
                            mimeType: "text/plain"
                            invokeActionId: "bb.action.SHARE"
                         } onTriggered: { data = onlineUrl.text; }
                },
                ActionItem {
                    id: offlineItem
                    objectName: "offlineItem"
                    title: "Offline View"
                    enabled: settingsmap.offlinemode?true:false;
                    imageSource: "asset:///images/offline.png"
                    ActionBar.placement: settingsmap.offlinemode?ActionBarPlacement.OnBar:ActionBarPlacement.InOverflow;
                    onTriggered: {
                        myWebView.url = offlineUrl.text;
                    }
                    
                },
                
                ActionItem {
                    id: favItem
                    objectName: "favItem"
                    title: "Favourite"
                    imageSource: "asset:///images/favourite.png"
                    ActionBar.placement: ActionBarPlacement.InOverflow
                    onTriggered: { addToFavourite(); }
                    shortcuts: [
                    	Shortcut {
                    		key: qsTr("F")
                    		onTriggered: { addToFavourite(); }
                    }]
                },
                ActionItem {
                    id: deleteItem
                    objectName: "deleteItem"
                    title: "Delete It"
                    imageSource: "asset:///images/delete.png"
                    ActionBar.placement: ActionBarPlacement.InOverflow
                    onTriggered: { deleteArticle(); }
                    shortcuts: [
                    	Shortcut {
                    		key: qsTr("D")
                    		onTriggered: { deleteArticle(); }
                    }]
                }
            ]
            
    onCreationCompleted: {  }
    
    function archive() {
        console.log("ID FOUND: " + currentID.text);
        myQmlCheck.show();
        app.checkmarkRequest("" + currentID.text);
        navigationPane.pop();
    }
    
    function deleteArticle() {
        console.log("ID FOUND: " + currentID.text);
        myQmlDeleteById.show();
        app.deleteRequest(currentID.text);
        navigationPane.pop();
    }
    
    function addToFavourite() {
        console.log("ID FOUND: " + currentID.text);
        myQmlFav.show();
        app.favouriteRequest(data);
        navigationPane.pop();
    }
    
    function copyLink() {
        myQmlPaste.show();
        app.toClipBoard(onlineUrl.text);
    }
    
}
