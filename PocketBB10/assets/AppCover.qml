import bb.cascades 1.0

Container {
    Container {
        layout: DockLayout {
        }
        background: Color.Black

        ImageView {
            imageSource: "asset:///images/aframe.png"
            scalingMethod: ScalingMethod.AspectFit
        }

       /* Container {
            bottomPadding: 31
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            Container {
                preferredWidth: 300
                preferredHeight: 42
                background: Color.create("#121212")
                layout: DockLayout {
                }

                Label {
                    objectName: "TheLabel"
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    text: "Hello!"
                    textStyle.color: Color.create("#ebebeb")
                    textStyle.fontSize: FontSize.PointValue
                    textStyle.fontSizeValue: 6

                }
            }
        }*/
    }
}