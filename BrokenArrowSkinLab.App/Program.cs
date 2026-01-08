using Microsoft.UI.Dispatching;
using Microsoft.UI.Xaml;
using Microsoft.Windows.AppLifecycle;
using System;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using Windows.ApplicationModel.Activation;

namespace BrokenArrowSkinLab.App;

public static class Program
{
    [STAThread]
    static void Main(string[] args)
    {
        WinRT.ComWrappersSupport.InitializeComWrappers();
        bool isRedirect = DecideRedirection();
        if (!isRedirect)
        {
            Application.Start((p) =>
            {
                var context = new DispatcherQueueSynchronizationContext(
                    DispatcherQueue.GetForCurrentThread());
                SynchronizationContext.SetSynchronizationContext(context);
                new App();
            });
        }
    }

    private static bool DecideRedirection()
    {
        bool isRedirect = false;
        AppActivationArguments args = AppInstance.GetCurrent().GetActivatedEventArgs();
        ExtendedActivationKind kind = args.Kind;
        AppInstance keyInstance = AppInstance.FindOrRegisterForKey("BrokenArrowSkinLabMain");

        if (keyInstance.IsCurrent)
        {
            keyInstance.Activated += OnActivated;
        }
        else
        {
            isRedirect = true;
            RedirectActivationTo(args, keyInstance);
        }
        return isRedirect;
    }

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
    private static extern IntPtr CreateEvent(
        IntPtr lpEventAttributes, bool bManualReset,
        bool bInitialState, string lpName);

    [DllImport("kernel32.dll")]
    private static extern bool SetEvent(IntPtr hEvent);

    [DllImport("ole32.dll")]
    private static extern uint CoWaitForMultipleObjects(
        uint dwFlags, uint dwMilliseconds, ulong nHandles,
        IntPtr[] pHandles, out uint dwIndex);

    private static IntPtr redirectEventHandle = IntPtr.Zero;

    private static void OnActivated(object? sender, AppActivationArguments args)
    {
        if (args.Kind == ExtendedActivationKind.Launch)
        {
            string[] launchArgs = args.Data as string[] ?? Array.Empty<string>();
            if (launchArgs.Any())
            {
                // Handle file opening or other activation args
            }
        }
    }

    private static void RedirectActivationTo(
        AppActivationArguments args, AppInstance keyInstance)
    {
        var redirectSemaphoreName = $"SkinLabRedirectSem{keyInstance.Key}";
        var redirectEventHandle = CreateEvent(IntPtr.Zero, false, false, redirectSemaphoreName);

        Task.Run(() =>
        {
            keyInstance.RedirectActivationToAsync(args).AsTask().Wait();
            SetEvent(redirectEventHandle);
        });

        uint CWMO_DEFAULT = 0;
        uint INFINITE = 0xFFFFFFFF;
        _ = CoWaitForMultipleObjects(
            CWMO_DEFAULT,
            INFINITE,
            1,
            new IntPtr[] { redirectEventHandle },
            out _);
    }
}
