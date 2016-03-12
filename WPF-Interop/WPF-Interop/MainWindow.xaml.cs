///////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Remote Performance Accessment client GUI //
//                                                               //
// Yufan Gong, CSE687 - Object Oriented Design, Spring 2014      //
///////////////////////////////////////////////////////////////////
/*
 * Remote Performance Accessment client GUI
 * 
 * version 1.0 : 29 Apr, 2014;
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;


namespace WPF_Interop
{
    public partial class MainWindow : Window
    {
        ITask mytask;
        int LocalPort;
        String LocalIp;
        String filename;
        List<int> portnumbers;

        //----< Main Window ctor >-------------------------------
        public MainWindow()
        {
            InitializeComponent();
            disableButtonsExceptConnect();
            portnumbers = new List<int>();
        }

        //----< disable all buttons except connect before connect >-------------------------------
        void disableButtonsExceptConnect()
        {
            button_connect.IsEnabled = true;
            button_download.IsEnabled = false;
            button_simi.IsEnabled = false;
            button_textsearch.IsEnabled = false;
            button_upload.IsEnabled = false;
            button_disconnect.IsEnabled = false;
            button_update.IsEnabled = false;
            button_browse.IsEnabled = false;
            button_downloadsearch.IsEnabled = false;
        }

        //----< disable all buttons >-------------------------------
        void disableButtons()
        {
            button_connect.IsEnabled = false;
            button_download.IsEnabled = false;
            button_upload.IsEnabled = false;
            button_simi.IsEnabled = false;
            button_textsearch.IsEnabled = false;
            button_disconnect.IsEnabled = false;
            button_update.IsEnabled = false;
            button_browse.IsEnabled = false;
            button_downloadsearch.IsEnabled = false;
        }

        //----< enable all buttons after connect >-------------------------------
        void enableButtons()
        {
            button_connect.IsEnabled = false;
            button_download.IsEnabled = true;
            button_upload.IsEnabled = true;
            button_simi.IsEnabled = true;
            button_textsearch.IsEnabled = true;
            button_disconnect.IsEnabled = true;
            button_update.IsEnabled = true;
            button_browse.IsEnabled = true;
            button_downloadsearch.IsEnabled = true;
        }

        //----< create a new instance of this client connect to server and get files from server >-------------------
        Task ConnectAndGetFiles()
        {
            // create instance of native task
            mytask = ITask.Create(LocalIp, LocalPort);

            // hook up callback if not done already

            //if (mytask.callback == null)
                mytask.callback = new ITask.EventCallBack(this.replyConnect);

            // run task asynchronously

            Task task = Task.Run(() => mytask.run(Dispatcher, "Files", "../../|*.*|/s|", "get files"));
            return task;
        }
       //---------<check is current port number has been used>-------------------------------
        private int checkPort(int portnumber)
        {
            while(portnumbers.Contains(portnumber))
            {
                portnumber++;
            }
            return portnumber;
        }

        //----< connect button >-------------------------------
        private async void button_connect_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (textbox_localip.Text != "127.0.0.1")
                {
                    System.Windows.MessageBox.Show("This test version can only accept 127.0.0.1 ip address.");
                    textbox_localip.Text = "127.0.0.1";
                }
                else if (textbox_localport.Text == "")
                {
                    System.Windows.MessageBox.Show("Please enter the port number of your local machine.");
                }
                else
                {
                    int portnumber = Convert.ToInt32(textbox_localport.Text);
                    portnumber = checkPort(portnumber);
                    portnumbers.Add(portnumber);
                    disableButtons();
                    LocalIp = textbox_localip.Text;
                    LocalPort = portnumber;
                    await ConnectAndGetFiles();
                    enableButtons();
                }
            }
            catch(Exception)
            {
                System.Windows.MessageBox.Show("Invalid input, check again!");
            }
        }

        //----< callback for listbox >-----------------------------------
        void replyConnect(String reply)
        {
            listbox_serverfiles.Items.Clear();
            string[] timeandfiles = reply.Split('@');
            textblock_endtoend.Text = timeandfiles[0];
            textblock_process.Text = timeandfiles[1];
            string[] files = timeandfiles[2].Split('|');
            foreach (var file in files)
            {
                if(file != "")
                 listbox_serverfiles.Items.Add(file);
            }
        }

        //----< do text search task >-----------------------------------
        Task TextSearch(String keyword, int threadNum)
        {
            //if (mytask.callback == null)
                mytask.callback = new ITask.EventCallBack(this.replyTextsearch);

            // run task asynchronously

            Task task = Task.Run(() => mytask.runSearch(Dispatcher, "TextSearch", "../|*.cpp *.h|/s|", keyword, threadNum));
            return task;
        }
        //----< search button >-------------------------------
        private async void button_textsearch_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                disableButtons();
                if (combobox_threadnumber.SelectedItem == null)
                {
                    System.Windows.MessageBox.Show("Please select thread number!");
                }
                else if (textbox_searchstring.Text == "")
                {
                    System.Windows.MessageBox.Show("Please enter text to search!");
                }
                else
                {
                    int threadNum = int.Parse(combobox_threadnumber.SelectedIndex.ToString()) + 1;
                    String keyword = textbox_searchstring.Text;
                    await TextSearch(keyword, threadNum);
                }
                enableButtons();
            }
            catch(Exception)
            {
                System.Windows.MessageBox.Show("Invalid input, check again!");
            }
        }

        //----< callback for status bar or message box >-----------------------------------
        void replyTextsearch(String SearchResults)
        {
            listbox_searchresults.Items.Clear();
            string[] timeandfiles = SearchResults.Split('@');

            textblock_endtoend.Text = timeandfiles[0];

            textblock_process.Text = timeandfiles[1];


            string[] searchresults = timeandfiles[2].Split('|');
            foreach(var result in searchresults)
            {
                if (result != "")
                listbox_searchresults.Items.Add(result);
            }
        }

        //----< do similarity analysis task >-----------------------------------
        Task SimiAna()
        {
            //if (mytask.callback == null)
                mytask.callback = new ITask.EventCallBack(this.replySimi);

            // run task asynchronously

            Task task = Task.Run(() => mytask.run(Dispatcher, "Similarity", "../|*.cpp *.h|/s /r /b|", "simianalysis"));
            return task;
        }

        //----< similarity button >-------------------------------
        private async void button_simi_Click(object sender, RoutedEventArgs e)
        {
            disableButtons();
            await SimiAna();
            enableButtons();
        }

        //----< callback for listbox >-----------------------------------
        void replySimi(String simiResults)
        {
            listbox_searchresults.Items.Clear();
            string[] timeandfiles = simiResults.Split('@');
            textblock_endtoend.Text = timeandfiles[0];

            textblock_process.Text = timeandfiles[1];

            textblock_simi.Text = timeandfiles[2];
        }

        //----< do get files names from server task >-----------------------------------
        Task GetFiles()
        {
            // create instance of native task

            mytask.callback = new ITask.EventCallBack(this.replyConnect);

            // run task asynchronously

            Task task = Task.Run(() => mytask.run(Dispatcher, "Files", "../|*.*|/s|", "get files"));
            return task;
        }

        //----< update list button >-------------------------------
        private async void button_update_Click(object sender, RoutedEventArgs e)
        {
            disableButtons();
            await GetFiles();
            enableButtons();
        }

        //----< do upload file task >-----------------------------------
        Task UploadFile(String selectedfile)
        {
            //if (mytask.callback == null)
            mytask.callback = new ITask.EventCallBack(this.replyUpload);

            // run task asynchronously
            int pos = selectedfile.LastIndexOf("\\");
            string shortfilename = selectedfile.Remove(0, pos + 1);

            Task task = Task.Run(() => mytask.run(Dispatcher, "Upload", selectedfile, shortfilename));
            return task;
        }

        //----< upload button >-------------------------------
        private async void button_upload_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                disableButtons();

                if (filename == null)
                {
                    System.Windows.Forms.MessageBox.Show("Choose a file");
                }
                else
                {
                    // int pos = filename.LastIndexOf("\\");
                    await UploadFile(filename);
                }
                textbox_filepathname.Text = null;
                filename = null;
                enableButtons();
            }
            catch (Exception)
            {
                System.Windows.MessageBox.Show("Invalid input, check again!");
            }
        }

        //----< callback for message box and status bar >-----------------------------------
        void replyUpload(String uploadresult)
        {
            listbox_searchresults.Items.Clear();
            string[] timeandfiles = uploadresult.Split('@');
            textblock_endtoend.Text = timeandfiles[0];

            textblock_process.Text = timeandfiles[1];

            System.Windows.MessageBox.Show(timeandfiles[2]);
            textblock_status.Text = timeandfiles[2];
        }

        //----< browse button >-------------------------------
        private void button_browse_Click(object sender, RoutedEventArgs e)
        {
            disableButtons();
            textbox_filepathname.Text = null;
            OpenFileDialog dlg = new OpenFileDialog();
            DialogResult result = dlg.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                filename = dlg.FileName;
            }
            textbox_filepathname.Text = filename;
            enableButtons();
        }

        //----< do download file task >-----------------------------------
        Task Download(String selectedfile)
        {
            //if (mytask.callback == null)
            mytask.callback = new ITask.EventCallBack(this.replyUpload);

            // run task asynchronously
            int pos = selectedfile.LastIndexOf("/");
            string shortfilename = selectedfile.Remove(0, pos + 1);

            Task task = Task.Run(() => mytask.run(Dispatcher, "Download", selectedfile, shortfilename));
            return task;
        }

        //----< download button >-------------------------------
        private async void button_download_Click(object sender, RoutedEventArgs e)
        {
            disableButtons();
            if(listbox_serverfiles.SelectedItem == null)
            {
                System.Windows.MessageBox.Show("Select a file to download");
            }
            else
            {
                String selecteddownload = listbox_serverfiles.SelectedItem.ToString();
                await Download(selecteddownload);
            }
            enableButtons();
        }

        //----< do Disconnect task >-----------------------------------
        Task Disconnect()
        {

            listbox_serverfiles.Items.Clear();


            Task task = Task.Run(() => mytask.run(Dispatcher, "Stop", "null", "null"));
            return task;
        }

        //----< disconnect button >-------------------------------
        private async void button_disconnect_Click(object sender, RoutedEventArgs e)
        {
            disableButtons();
            await Disconnect();
            disableButtonsExceptConnect();
        }

        //----< download from search result list button >-------------------------------
        private async void button_downloadsearch_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                disableButtons();
                if (listbox_searchresults.SelectedItem == null)
                {
                    System.Windows.MessageBox.Show("Select a file to download");
                }
                else if (listbox_searchresults.SelectedItem.ToString() == "No items match your search.")
                {
                    System.Windows.MessageBox.Show("Select a file to download");
                }
                else
                {
                    String selecteddownload = listbox_searchresults.SelectedItem.ToString();
                    await Download(selecteddownload);
                }
                enableButtons();
            }
            catch (Exception)
            {
                System.Windows.MessageBox.Show("Invalid input, check again!");
            }
        }
    }
}