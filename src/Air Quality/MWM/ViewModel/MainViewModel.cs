using System;
using AirQuality.Core;

namespace AirQuality.MWM.ViewModel
{
    class MainViewModel : ObservableObject
    {

        private object _currentView;

        public HomeViewModel HomeVM { get; set; }

        public object CurrentView
        {
            get { return _currentView; }
            set
            {
                _currentView = value;
                OnPropertyChanged();
            }
        }


        public MainViewModel() 
        {
            HomeVM = new HomeViewModel();
            CurrentView = HomeVM;
        }

    }

}
