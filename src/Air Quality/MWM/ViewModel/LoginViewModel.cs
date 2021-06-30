namespace AirQuality.MWM.ViewModel
{
    class LoginViewModel
    {
        private string ipAddress;
        private MainViewModel mainViewModel;

        public LoginViewModel()
        {
        }

        public LoginViewModel(MainViewModel mainViewModel)
        {
            this.mainViewModel = mainViewModel;
        }

        public string IpAddress { get => ipAddress; set => ipAddress = value; }
        public MainViewModel MainViewModel { get => mainViewModel; set => mainViewModel = value; }
    }
}

