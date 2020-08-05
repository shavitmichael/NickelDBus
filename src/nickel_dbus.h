#ifndef NICKEL_DBUS_H
#define NICKEL_DBUS_H
#include <QDialog>
#include <QObject>
#include <QString>
#include <QSet>
#include <QtDBus>
#include <QDBusContext>

typedef void PlugManager;
typedef QObject PlugWorkflowManager;
typedef QObject WirelessManager;
typedef void MainWindowController;
typedef QDialog ConfirmationDialog;

#ifndef NDB_DBUS_IFACE_NAME
    #define NDB_DBUS_IFACE_NAME "com.github.shermp.nickeldbus"
#endif
#ifndef NDB_DBUS_OBJECT_PATH
    #define NDB_DBUS_OBJECT_PATH "/nickeldbus"
#endif
/*!
 * \brief Register a service on the d-bus system bus on Kobo e-reader devices
 * 
 */
class NickelDBus : public QObject, protected QDBusContext {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", NDB_DBUS_IFACE_NAME)

    QDBusConnection conn = QDBusConnection::systemBus();
    
    public:
        bool initSucceeded;
        /*!
         * \brief Construct a new Nickel D-Bus object
         * 
         * \param parent 
         */
        NickelDBus(QObject* parent);
        /*!
         * \brief Destroy the Nickel D-Bus object
         * 
         */
        ~NickelDBus();
        // bool registerDBus();
        /*!
         * \brief Connects available Nickel signals to d-bus
         * 
         * Failures to connect a signal will stop execution, the failure will be logged
         * to syslog. 
         */
        void connectSignals();

    Q_SIGNALS:
        /*!
         * \brief The signal that is emitted when a confirmation dialog button is tapped, or the dialog is closed
         * 
         * \param result will be 0 for REJECT or 1 for ACCEPT
         */
        void dlgConfirmResult(int result);
        // PlugworkFlowManager signals
        /*!
         * \brief The signal that nickel emits when the content import process has completed.
         * 
         * The signal will be emitted following the content import triggered whenever 
         * the user unplugs from the computer, when rescan_books/rescan_books_full 
         * actions are triggered from NickelMenu, or when pfmRescanBooks/pfmRescanBooksFull
         * methods are called from NickelDBus.
         */
        void pfmDoneProcessing();
        /*!
         * \brief The signal that nickel emits when it is about to start the USB connection
         * 
         */
        void pfmAboutToConnect();
        // WirelessManager signals
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmTryingToConnect();
        /*!
         * \brief This signal appears to be emitted when the network has successfully connected
         * 
         * I'm unsure if this is emitted when the WiFi connects, or when a valid IP address
         * is obtained.
         */
        void wmNetworkConnected();
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmNetworkDisconnected();
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmNetworkForgotten();
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmNetworkFailedToConnect();
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmScanningStarted();
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmScanningFinished();
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmScanningAborted();
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmWifiEnabled(bool enabled);
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmLinkQualityForConnectedNetwork(double quality);
        /*!
         * \brief (todo: figure this out)
         * 
         */
        void wmMacAddressAvailable(QString mac);

    public Q_SLOTS:
        /*!
         * \brief Returns the version of NickelDBus, as a string.
         * 
         * \return QString 
         */
        QString ndbVersion();
        /*!
         * \brief Tests if the current version of NickelDBus satisfies vers
         * 
         * \param vers the version to test
         * \return true if the current version of NickelDBus is >= the requested version
         * \return false 
         */
        bool ndbSatisfiesVersion(QString vers);
        /*!
         * \brief Print available details from nickel classes
         * 
         * This method attempts to dlsym then parse the staticMetaObject
         * property available in many of the classes. In most cases, the
         * available signals and slots can be determined
         * 
         * \param static_metaobject_symbol the "mangled" symbol of the staticMetaObject property for a class
         * \return QString Formatted output of the available details (signals and slots)
         */
        QString miscNickelClassDetails(QString const& static_metaobject_symbol);
        // misc
        /*!
         * \brief Check if a signal was successfully connected
         * 
         * \param signal_name the signal name, sans parentheses and parameters
         * \return true 
         * \return false 
         */
        bool miscSignalConnected(QString const& signal_name);
        /*!
         * \brief Show a small, temporary text box with a message 
         * 
         * \param toast_duration in milliseconds, required
         * \param msg_main the main message to display
         * \param msg_sub a sub-message to display, optional and may be omitted
         */
        void mwcToast(int toast_duration, QString const& msg_main, QString const& msg_sub = QStringLiteral(""));
        /*!
         * \brief Navigate to the home screen
         * 
         */
        void mwcHome();
        // Confirmation Dialogs
        /*!
         * \brief Show a confirmation dialog with no buttons (except close)
         * 
         * When the dialog is closed, a dlgConfirmResult(result_code) signal is emitted.
         * result_code will be REJECT (0)
         * 
         * \param title Title of the dialog
         * \param body Body text of the dialog
         */
        void dlgConfirmNoBtn(QString const& title, QString const& body);
        /*!
         * \brief Show a confirmation dialog with an accept button
         * 
         * When the accept button is tapped, or the dialog is closed, a dlgConfirmResult(result_code) signal is emitted.
         * result_code will be ACCEPT (1) if accept button tapped, otherwise REJECT (0)
         * 
         * \param title Title of the dialog
         * \param body Body text of the dialog
         * \param acceptText The label of the accept button (eg: 'accept', 'ok', 'yes')
         */
        void dlgConfirmAccept(QString const& title, QString const& body, QString const& acceptText);
        /*!
         * \brief Show a confirmation dialog with reject button
         * 
         * When the reject button is tapped, or the dialog is closed, a dlgConfirmResult(result_code) signal is emitted.
         * result_code will be REJECT (0)
         * 
         * \param title Title of the dialog
         * \param body Body text of the dialog
         * \param rejectText The label of the reject button (eg: 'reject', 'cancel', 'no')
         */
        void dlgConfirmReject(QString const& title, QString const& body, QString const& rejectText);
        /*!
         * \brief Show a confirmation dialog with both accapt and reject buttons
         * 
         * When either button is tapped, or the dialog is closed, a dlgConfirmResult(result_code) signal is emitted.
         * result_code will be ACCEPT (1) if accept button tapped, otherwise REJECT (0)
         * 
         * \param title Title of the dialog
         * \param body Body text of the dialog
         * \param acceptText The label of the accept button (eg: 'accept', 'ok', 'yes')
         * \param rejectText The label of the reject button (eg: 'reject', 'cancel', 'no')
         */
        void dlgConfirmAcceptReject(QString const& title, QString const& body, QString const& acceptText, QString const& rejectText);
        // PlugWorkFlowManager
        /*!
         * \brief Begin an abbreviated book rescan. Same as 'rescan_books' from NickelMenu
         */
        void pfmRescanBooks();
        /*!
         * \brief Begins a full book rescan. Same as 'rescan_books_full' from NickelMenu
         */
        void pfmRescanBooksFull();
        // Wireless methods (WirelessFlowManager)
        /*!
         * \brief Connect to WiFi network. Same as 'autoconnect' option from NickelMenu
         */
        void wfmConnectWireless();
        /*!
         * \brief Connect silently to WiFi network. Same as 'autoconnect_silent' from NickelMenu
         */
        void wfmConnectWirelessSilently();
        /*!
         * \brief Enable/disable/toggle WiFi. Same as NickelMenu WiFi 'enable'/'disable'/'toggle' options
         * 
         * \param action string, one of 'enable', 'disable', 'toggle'
         */
        void wfmSetAirplaneMode(QString const& action);
        // Web Browser (BrowserWorkflowManager)
        /*!
         * \brief Open the web browser. Same as NickelMenu browser options
         * 
         * \param modal boolean, select whether the browser should open as a modal or not
         * \param url string, an optional URL to open
         * \param css string, optional CSS to set
         */
        void bwmOpenBrowser(bool modal = false, QString const& url = QString(), QString const& css = QString());
        // Nickel Settings
        /*!
         * \brief Invert the screen
         * 
         * \param action string, one of 'enable', 'disable', 'toggle'
         */
        void nsInvert(QString const& action);
        /*!
         * \brief Set UnlockEnabled
         * 
         * \param action string, one of 'enable', 'disable', 'toggle'
         */
        void nsLockscreen(QString const& action);
        /*!
         * \brief Enable screenshots
         * 
         * \param action string, one of 'enable', 'disable', 'toggle'
         */
        void nsScreenshots(QString const& action);
        /*!
         * \brief Sets the developer ForceWifiOn setting
         * 
         * \param action string, one of 'enable', 'disable', 'toggle'
         */
        void nsForceWifi(QString const& action);
        // Power commands
        /*!
         * \brief Shutdown the Kobo
         */
        void pwrShutdown();
        /*!
         * \brief Reboot the Kobo
         */
        void pwrReboot();
    protected Q_SLOTS:
        void allowDialog();
    private:
        void *libnickel;
        bool allowDlg = true;
        QSet<QString> connectedSignals;
        
        struct {
            bool *(*PlugManager__gadgetMode)(PlugManager*);
            PlugManager *(*PlugManager__sharedInstance)();
            PlugWorkflowManager *(*PlugWorkflowManager_sharedInstance)();
            WirelessManager *(*WirelesManager_sharedInstance)();
            ConfirmationDialog *(*ConfirmationDialogFactory_getConfirmationDialog)(QWidget*);
            void (*ConfirmationDialog__setTitle)(ConfirmationDialog* _this, QString const&);
            void (*ConfirmationDialog__setText)(ConfirmationDialog* _this, QString const&);
            void (*ConfirmationDialog__setAcceptButtonText)(ConfirmationDialog* _this, QString const&);
            void (*ConfirmationDialog__setRejectButtonText)(ConfirmationDialog* _this, QString const&);
            MainWindowController *(*MainWindowController_sharedInstance)();
            void (*MainWindowController_toast)(MainWindowController*, QString const&, QString const&, int);
        } nSym;

        void ndbResolveSymbol(const char *name, void** sym);
        bool ndbInUSBMS();
        bool ndbActionStrValid(QString const& actStr);
        void ndbWireless(const char *act);
        void ndbSettings(QString const& action, const char* setting);
        void ndbNickelMisc(const char *action);
        QString getNickelMetaObjectDetails(const QMetaObject* nmo);
        template <typename T>
        void ndbConnectSignal(T *srcObj, const char *srcSignal, const char *dest);
        void dlgConfirmation(QString const& title, QString const& body, QString const& acceptText, QString const& rejectText);
        void pwrAction(const char *action);
};

#endif