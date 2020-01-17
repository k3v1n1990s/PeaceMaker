#include "ClickableTab.h"
#include "mainwindow.h"

ClickableTab::ClickableTab(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    std::string parentName;
    QWidget* currentParent;

    //
    // Find the MainWindow.
    //
    currentParent = parent;
    do
    {
        currentParent = currentParent->parentWidget();
        parentName = currentParent->objectName().toStdString();
    } while(parentName != "MainWindow");

    this->mainWindow = currentParent;

    this->tabActive = false;
}

ClickableTab::~ClickableTab() {}

void ClickableTab::SwapActiveState()
{
    int statusPosition;
    std::string currentTabContent;
    //
    // These values are what we look for in the tab HTML to replace.
    //
    const std::string activeName = "Active";
    const std::string inactiveName = "Inactive";

    //
    // Get the current tab content.
    //
    this->text();
    currentTabContent = this->text().toStdString();
    //
    // Replace the active state depending on whether it was already active.
    //
    if(this->tabActive)
    {
        statusPosition = currentTabContent.find(activeName);
        currentTabContent.replace(statusPosition, activeName.length(), inactiveName);
        this->tabActive = false;
        for(QWidget* widget : this->associatedElements)
        {
            widget->setVisible(false);
        }
    }
    else
    {
        statusPosition = currentTabContent.find(inactiveName);
        currentTabContent.replace(statusPosition, inactiveName.length(), activeName);
        this->tabActive = true;
        for(QWidget* widget : this->associatedElements)
        {
            widget->setVisible(true);
        }
    }

    //
    // Set this new text.
    //
    this->setText(QString::fromStdString(currentTabContent));
}

/**
 * @brief ClickableTab::AddAssociatedElement - Track associated widgets to hide/show on state changes.
 * @param widget - The widget associated with this tab.
 */
void ClickableTab::AddAssociatedElement(QWidget *widget)
{
    this->associatedElements.push_back(widget);

    //
    // By default, widgets should be hidden.
    //
    widget->setVisible(false);
}

/**
 * @brief ClickableTab::mousePressEvent notifies the main window of a tab being clicked.
 * @param event - Details about the click event.
 */
void ClickableTab::mousePressEvent(QMouseEvent* event) {
    ((MainWindow*)this->mainWindow)->NotifyTabClick(this);
}
