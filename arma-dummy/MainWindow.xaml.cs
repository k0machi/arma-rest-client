﻿using System;
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
using System.Runtime.InteropServices;

namespace arma_dummy
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void BtnArgs_Click(object sender, RoutedEventArgs e)
        {
            StringBuilder output = new StringBuilder(10240);
            string func = argCall_func_name.Text;
            // Arma 3 will send arrays of arrays as arrays of their textual representation, so splitting by , will also split those textual representations
            // so as to simulate output from A3 we'll have to omit outmost brackets and separate arrays containing sub-arrays by some impossible separator like ,,
            // "http://google.com",, [["token", "abcd"], ["name", "alice"], ["sort", "asc"] ],, [ ["encode", "urlencode"], ["something", "else"] ]
            // is equal to
            // ["http://google.com", [["token", "abcd"], ["name", "alice"], ["sort", "asc"] ], [ ["encode", "urlencode"], ["something", "else"] ]]
            string[] args = argCall_argv.Text.Split(new string[] { ",," }, StringSplitOptions.None);
            ReturnCode.Text = (RVCommand.RVExtensionArgs(output, output.Capacity, func, args, args.Length)).ToString();
            ResultFld.Text = output.ToString();
        }

        private void BtnFunc_Click(object sender, RoutedEventArgs e)
        {
            StringBuilder output = new StringBuilder(10240);
            string func = funcCall_func_name.Text;
            RVCommand.RVExtension(output, output.Capacity, func);
            ResultFld.Text = output.ToString();
        }

        private void VersionBtn_Click(object sender, RoutedEventArgs e)
        { 
            StringBuilder output = new StringBuilder(10240);
            RVCommand.RVExtensionVersion(output, output.Capacity);
            ResultFld.Text = output.ToString();
        }

    }
}
