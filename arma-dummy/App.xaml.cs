using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Runtime.InteropServices;
using System.Text;

namespace arma_dummy
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
    }

    public static class RVCommand
    {
        [DllImport(".\\extension_x64.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void RVExtension(StringBuilder output, int outputSize, string function);
        [DllImport(".\\extension_x64.dll", CallingConvention = CallingConvention.StdCall, ExactSpelling = true)]
        public static extern void RVExtensionArgs(StringBuilder output, int outputSize, string function, string[] argv, int argc);
        [DllImport(".\\extension_x64.dll", CallingConvention = CallingConvention.StdCall, ExactSpelling = true)]
        public static extern void RVExtensionVersion(StringBuilder output, int outputSize);
    }

}
