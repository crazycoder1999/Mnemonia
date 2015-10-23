import bb.cascades 1.0
import bb.system 1.0
import custom 1.0
import "globalvar.js" as Gvar
NavigationPane {
    id: navigationPane
    peekEnabled: false
        Menu.definition: MenuDefinition { //se ho problemi posso aggiungerlo anche alla sola pagina..
            settingsAction: SettingsActionItem {
                  onTriggered : {
                      var page = settingsPage.createObject();
                      navigationPane.push(page);    
                  }
                }
        } // end of MenuDefinition
Page {
    property alias sqldata: sqldata    

    Container {
        layout: StackLayout {orientation: LayoutOrientation.TopToBottom}
        background: settingsmap.isblack?Color.create("#150517"):Color.create ("#F1E3AD")
        Container {
            layout: AbsoluteLayout { }
            preferredHeight: 60
            Label {
                layoutProperties: AbsoluteLayoutProperties {
                            positionX: 15
                            positionY: 15
                }
                text: "Mnemonia"
                textStyle {
                      base: SystemDefaults.TextStyles.BodyText
                      fontWeight: FontWeight.Bold
                      fontSize: FontSize.Large
                      color: settingsmap.isblack?Color.create("#444444"):Color.create("#A06C42")
                }
            }
        }
        Divider { }
        Container { 
                // Create a ListView that uses an XML data model
                layout: StackLayout {orientation: LayoutOrientation.TopToBottom}
                horizontalAlignment: HorizontalAlignment.Center;
                background: settingsmap.isblack?Color.create("#000000"):Color.create ("#FFFFFF")
                Button {  
                    objectName: "btnCode"
                    id: btnCode              
                    text: "Login"
                    imageSource: "asset:///images/account.png"
                    onClicked: {
                        app.askCode();
                        visible = false;
                        //btnToken.enabled = true;
                    }
                }
                Button {
                    objectName: "btnToken"
                    id: btnToken   
                    enabled: false 
                    visible: false
                    text: "Finalize Login"
                    imageSource: "asset:///images/account.png"
                    onClicked: {
                            app.askToken();
                            myListView.visible = true;
                            visible = false;
                            syncBtn.enabled = true;
                            logoutBtn.enabled = true;
                            addItem.enabled = true;
                    }
                }
                Label {
                    objectName: "labelProblem"
                    visible: false
                    text: "There's a problem with login..\nPlease close the app and try again.\nCheck the help for more information"
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        fontWeight: FontWeight.Normal
                        fontSize: FontSize.Medium
                        color: Color.create("#FF0000")
                    }
                    multiline: true
                }
                ListView {
                    dataModel:sqldata
                    id: myListView
                    objectName: "myListView"
                    visible: false 
                    listItemComponents: [
                        
                        ListItemComponent {
                            type: "item" 
                            
                            Container {

                                id: aRow;   
                                layout: StackLayout { }
                                leftPadding: 15
                                rightPadding: 15
                                bottomPadding: 15
                                topPadding: 5
                                Label {
                                    id:titleItem
                                    text: ListItemData.TITLE
                                    textStyle {
                                            base: SystemDefaults.TextStyles.TitleText
                                            fontWeight: FontWeight.Bold
                                            fontSize:FontSize.Medium
                                            color:  aRow.ListItem.view.isBlack()?Color.create("#E0E0E0"):Color.create("#000000")
                                    }
                                    topMargin:2
                                    bottomMargin:2
                                    multiline: true
                                }
                                Label {
                                    text: aRow.ListItem.view.cutText(ListItemData.DESC)
                                    textStyle {
                                        base: SystemDefaults.TextStyles.SubtitleText
                                        fontWeight: FontWeight.Normal
                                        fontSize: FontSize.Small
                                        color: aRow.ListItem.view.isBlack()?Color.create("#B0B0B0"):Color.create("#000000")
                                    }            
                                    topMargin:2
                                    bottomMargin:2
                                    multiline: true
                                }
                                Divider { }
                                attachedObjects: [
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
                                        }]
                                contextActions: [
                                            ActionSet {
                                                title: "Actions"
                                                subtitle: "Choose the operation"
                                                ActionItem {
                                                    title: "Archive it"
                                                    imageSource: "asset:///images/checkmark.png"
                                                    onTriggered: { 
                                                        console.log(ListItemData.ID);
                                                        myQmlCheck.show();
                                                        aRow.ListItem.view.checkMe(ListItemData.ID);
                                                    }
                                                }
                                                ActionItem {
                                                    title: "Delete it"
                                                    imageSource: "asset:///images/delete.png"
                                                    onTriggered: { 
                                                        console.log(ListItemData.ID);
                                                        myQmlDeleteById.show();
                                                        aRow.ListItem.view.deleteMe(ListItemData.ID);
                                                    }
                                                }
                                                ActionItem {
                                                    title: "Open in New Browser"
                                                    imageSource: "asset:///images/browser.png"
                                                    onTriggered: { 
                                                            aRow.ListItem.view.newBrowser(ListItemData.LINK);
                                                                                                      //  sqldata.removeItemAtIndex(index);
                                                    }
                                                }
                                                ActionItem {
                                                    title: "Favourite"
                                                    imageSource: "asset:///images/favourite.png"
                                                    onTriggered: { 
                                                        console.log("favourite!");    
                                                        myQmlFav.show();
                                                        aRow.ListItem.view.favMe(ListItemData.ID);
                                                    }
                                                }
                                                InvokeActionItem {
                                                         title: "Share"
                                                         query {
                                                                mimeType: "text/plain"
                                                                invokeActionId: "bb.action.SHARE"
                                                        } onTriggered: {
                                                            console.log("data " + data);
                                                                data = ListItemData.LINK;
                                                        }
                                                }
                                            } // end of ActionSet
                                ] // end of contextActions list            
                            }
                        } // end of second ListItemComponent
                    ] // end of listItemComponents list
                    function itemType(data, indexPath) {
                                    return "item";
                                }
                    onTriggered: {
                        var selectedItem = dataModel.data(indexPath);
                        Gvar.lastLink = selectedItem.LINK;
                        Gvar.lastItemID = selectedItem.ID;
                        var page = pageDefinition.createObject();
                        navigationPane.push(page);
                    }
                    function deleteMe(data) {
                       app.deleteRequest(data);
                    }
                    function isBlack(){
                        return settingsmap.isblack;
                    }
                    function checkMe(data) {
                       app.checkmarkRequest(data);
                    }
                   
                    function newBrowser(theUrl){
                        app.launchBrowserTo(theUrl);
                    }
                    function favMe(data){
                        app.favouriteRequest(data);
                    }
                    function cutURL(theUrl) {
                            if(theUrl.indexOf("http://") != -1) {
                      		    theUrl = theUrl.substring(7);
                      		}
                  	   		if(theUrl.indexOf("https://") != -1) {
                  	    	    theUrl = theUrl.substring(8);
                       	    }
                  	    	if(theUrl.indexOf("www.") != -1) {
                  		   	    theUrl = theUrl.substring(theUrl.indexOf("www.")+4);
                  		   	}
                  		   	if(theUrl.indexOf("/") != -1) {
                  		   	    theUrl = theUrl.substring(0,theUrl.indexOf("/"));
                  		   	}
                  		   	if(theUrl.length > 25) {
                  		   	    var z = theUrl.split(".");
                  		   	    if(z.length>2) {
                  		   	        theUrl = z[z.length-2] + "." + z[z.length-1];
                  		   	    }
                  		   	}
                  		return theUrl;
                  }
                  function cutText(txt) {
                      if(txt.length>100)
                          return txt.substr(0,130) + "...";
                      return txt;
                  }
                } // end of ListView
                attachedObjects: [
                      MySqlDataModel {
                          id: sqldata
                          onInitialItemsLoaded: { 
                              myListView.scrollToItem([0]);
                          }
                }]
            } // end of Container
    } //end of first Container
    
    actions: [
            ActionItem {
                id: addItem
                objectName: "addItem"
                title: "ADD Link"
                imageSource: "asset:///images/add.png"
                enabled: false;
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    console.log("add");
                    app.askUrl();    
                }
            },
            ActionItem {
                title: "Info"
                imageSource: "asset:///images/ic_info.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    console.log("ok");
                    var page = aboutPage.createObject();
                    navigationPane.push(page);
                }
            },
            ActionItem {
                title: "Help"
                imageSource: "asset:///images/ic_help.png"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                    console.log("ok");
                        var page = tutorialPage.createObject();
                        navigationPane.push(page);                          
                    }
                },  
            ActionItem {
                    title: "Sync"
                    id: syncBtn
                    objectName: "syncBtn"
                    imageSource: "asset:///images/sync.png"
                    enabled: false;
                    ActionBar.placement: ActionBarPlacement.OnBar
                            attachedObjects: [
                                  SystemToast {
                                        id: myQmlSync
                                        body: "Syncing..."
                            }]
                            onTriggered: {
                                    myQmlSync.show();
                                    app.retrieveList();                                             
                            }
                        },
            ActionItem {
                title: "Logout"
                 id: logoutBtn
                objectName: "logoutBtn"
                enabled: false;
                imageSource: "asset:///images/account.png"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                        console.log("logout");
                        app.logout();                        
                }
            }   
        ]
} //end of page
    onCreationCompleted: {
        app.mySignal.connect(doSomething);
        app.cleanList.connect(cleanList);
        app.removeItem.connect(removeItem);
        sqldata.loadData(articleOrdering());
       // titleItemColor = Color.White;
    }
    attachedObjects: [ComponentDefinition {
                        id: pageDefinition;
                        source: "showpage.qml";
                    },ComponentDefinition {
                        id: aboutPage;
                        source: "about.qml";
                    },ComponentDefinition {
                        id: tutorialPage;
                        source: "tutorial.qml";
                    },ComponentDefinition {
                        id: settingsPage;
                        source: "settings.qml";
                    }]
    onPopTransitionEnded: {
       // console.log("PAGE: " + page.objectName);
    	if(page.objectName == "settingspage") {
    	    if(settingsmap.orderby != page.ordering.selectedIndex ) {
    	        settingsmap.orderby = page.ordering.selectedIndex;
    	       // console.log("ORDERING IS CHANGED! Reload!");
                sqldata.removeItems();
                syncFromDb();
            }
    	}
        page.destroy(); 
    }
    onTopChanged: { 
            if(page.objectName == "showpage") {
                page.myWebView.url = Gvar.lastLink;
                page.offlineUrl.text = dirPaths.home +"/data/offline/"+ Gvar.lastItemID; 
                page.currentID.text = Gvar.lastItemID;
                page.onlineUrl.text = Gvar.lastLink;
                page.Gvar.lastItemID = Gvar.lastItemID;
            }
    }
    
    function syncFromDb() {
        console.log("syncing.. from db");
        sqldata.loadData(articleOrdering());
    }
    
    function articleOrdering() {
        switch (settingsmap.orderby) {
            case 0:
                return "id ASC";
            case 1:
                return "id DESC";
            case 2:
                return "title ASC";
            case 3:
                return "title DESC";
            default:
                return "id ASC";  
        }
    }
    function removeItem(id) {
        sqldata.removeItemWithId(id, true);
    }
    
    function doSomething(id,title,desc,lnk) {
        sqldata.insertLink(id,title,desc,lnk);
    }
    
    function cleanList() {
        sqldata.deleteAll();
        sqldata.removeItems();
    }
}//end of Navigation pane
