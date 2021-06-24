namespace AirQuality.MWM.ViewModel
{
    class LoginViewModel
    {
        private string ipAddress;//   The servers hostname or IP address need //need to find a way to get it  
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

