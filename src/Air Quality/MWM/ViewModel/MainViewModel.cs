using System;
using AirQuality.Core;

namespace AirQuality.MWM.ViewModel
{
    class MainViewModel : ObservableObject
    {

        public RelayCommand HomeViewCommand { get; set; }
        public RelayCommand SettingsViewCommand { get; set; }

        private object _currentView;

        public HomeViewModel HomeVM { get; set; }
        public LoginViewModel LoginVM { get; set; }
        public SettingsViewModel SettingsVM { get; set; }

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
            LoginVM = new LoginViewModel();
            SettingsVM = new SettingsViewModel();

            CurrentView = LoginVM;

            HomeViewCommand = new RelayCommand(o => 
            {
                CurrentView = HomeVM;
            });

            SettingsViewCommand = new RelayCommand(o =>
            {
                CurrentView = SettingsVM;
            });
        }

    }

}
