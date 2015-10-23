import bb.cascades 1.0

Page {
    objectName: "settingspage"
    property alias ordering: ordering;
    Container {
            leftPadding:20;
            rightPadding: 10;
            topPadding: 10;
            layout: StackLayout {orientation: LayoutOrientation.TopToBottom}
            Container{
            Label { 
                     text: "Settings"
                     textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                             fontWeight: FontWeight.Bold
                            fontSize: FontSize.Large
                    }
                }
            }
            Divider { }
            Container {
                 layout: StackLayout {orientation: LayoutOrientation.TopToBottom}
                 Container{
                     layout: DockLayout{}

                 Label {
                    horizontalAlignment: HorizontalAlignment.Left
                     verticalAlignment: VerticalAlignment.Center  
                     preferredWidth:  720;
                     text: "Black Theme"
                         textStyle {
                         base: SystemDefaults.TextStyles.BodyText
                         fontWeight: FontWeight.Bold
                         fontSize: FontSize.Medium
                      } 
                 }
                 ToggleButton {
                     horizontalAlignment: HorizontalAlignment.Right
                     verticalAlignment: VerticalAlignment.Center  
                     id: blacktheme
                     onCheckedChanged: {
                        saveAll();
                        changed.visible = true;
                    }
                 }
            }
            Divider { }
                 Container{
                     layout: DockLayout{}
                       Label {
                       		horizontalAlignment: HorizontalAlignment.Left
                            verticalAlignment: VerticalAlignment.Center  
                            preferredWidth:  720;
                            text: "Offline Mode";
                            textStyle {
                            	 base: SystemDefaults.TextStyles.BodyText
                            	 fontWeight: FontWeight.Bold
                            	 fontSize: FontSize.Medium
                            } 
                        }
                        ToggleButton {
                          	id: offlinemode
                          	horizontalAlignment: HorizontalAlignment.Right
                          	verticalAlignment: VerticalAlignment.Top  
                            onCheckedChanged: { 
                                saveAll();
                            	changed.visible = true;
                            }
                        }
                }
            Divider {
            }
            Container {
                layout: DockLayout {
                }
                
                DropDown {
                    id: ordering;
                    
                	Option {
                    	text: "Articles Order By Date";
                    	value: "dateasc";
                    }
                	Option {
                        text: "Articles Order By Date(Inverted)"
                        value: "datediscending"
                    }
                	Option {
                	    text: "Articles Ordered By Title"
                	    value: "titleasc"
                	}
                    Option {
                        text: "Articles Ordered By Title(Inverted)"
                        value: "titledesc"
                    }
                    onSelectedValueChanged: {
                        if( selectedValue != null ){
                            saveAll();
                        }
                    }
                }
            }
            Divider { }
            Label {
                id: changed;
                text: "You need to close and open again the app, in order to apply the new changes!";
                multiline: true;
                visible: false;
            }
        }
    } onCreationCompleted:  {
        offlinemode.checked = settingsmap.offlinemode?true:false;
        blacktheme.checked = settingsmap.isblack?true:false;
   //     console.log("ORDER VALUE" + settingsmap.orderby);
        ordering.setSelectedIndex(settingsmap.orderby);
        changed.visible=false;
    }
    
    function saveAll() {
        app.saveSettings(blacktheme.checked ? 1 : 0, offlinemode.checked ? 1 : 0,ordering.selectedIndex,-1);
    }
}
