from django.test import TestCase
from rest_framework.test import APILiveServerTestCase, APITestCase
import requests
import json
from api_trading.test import TradingBaseTestCase
from django.contrib.auth.models import User
from configuration.models import BotConfig


class BotConfigTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupUser();
        self.setupToken();
    

    def test_register_bot_configuration(self):
        """
        Test to verify the create bot configuration
        """
        url = self.get_url_server()+"api/config/bot/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "bot_status": True,
            "time_interval": 3600,
            "max_lost": "150000.0000000000",
            "db_verbosity": "medium"
            }
        #print(header)
        #print(data)
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(201, response.status_code)
        self.assertTrue(('id', 'url', 'bot_status','time_interval','max_lost','db_verbosity' in json.loads(response.content)))

class BotConfigOtherTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupUser();
        self.setupToken();
        url = self.get_url_server()+"api/config/bot/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "bot_status": True,
            "time_interval": 3600,
            "max_lost": "150000.0000000000",
            "db_verbosity": "medium"
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(201, response.status_code)
        self.botConfig_id=response.json()["id"]
        self.botConfig_url=response.json()["url"]
        

    def test_register_bot_configuration_with_other_in_status_true(self):
        
        url = self.get_url_server()+"api/config/bot/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "bot_status": True,
            "time_interval": 7200,
            "max_lost": "100000.0000000000",
            "db_verbosity": "silent"
            }      

        response = requests.post(url,headers=header,json=data)
        self.assertEqual(400, response.status_code)
        self.assertTrue("There is a configuration of the Bot" in json.loads(response.content))

    
    def test_update_bot_configuration_already_registered(self):
        """
        Test to verify the update bot configuration
        """      
        url = self.get_url_server()+"api/config/bot/"+str(self.botConfig_id)+"/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "id": self.botConfig_id,
            "bot_status": False,
            "time_interval": 3600,
            "max_lost": "150000.0000000000",
            "db_verbosity": "medium"
            }
        #print(url)
        #print(data)
        response = requests.put(url,headers=header,json=data)
        self.assertEqual(200, response.status_code)
        self.assertTrue(('id', 'url', 'bot_status','time_interval','max_lost','db_verbosity' in json.loads(response.content)))
        self.assertEqual(json.loads(response.content), {"id": self.botConfig_id,"url": self.botConfig_url, "bot_status": False, "time_interval": 3600, "max_lost": "150000.0000000000", "db_verbosity": "medium"})

    def test_update_specific_field_bot_configuration(self):

        url = self.get_url_server()+"api/config/bot/"+str(self.botConfig_id)+"/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "max_lost": "250000.0000000000",
            }
        
        response = requests.patch(url,headers=header,json=data)
        self.assertEqual(200, response.status_code)
        self.assertTrue(('id', 'url', 'bot_status','time_interval','max_lost','db_verbosity' in json.loads(response.content)))

    
class BotConfigQueryTestCase(TradingBaseTestCase):
    
    def setUp(self):
        self.setupUser();
        self.setupToken();

        url = self.get_url_server()+"api/config/bot/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "bot_status": True,
            "time_interval": 3600,
            "max_lost": "150000.0000000000",
            "db_verbosity": "medium"
            }

        response = requests.post(url,headers=header,json=data)

    def test_query_all_bot_configuration(self):
        """
        Test to verify transactions queries already created
        """
        url = self.get_url_server()+"api/config/bot/"
        header = {'Authorization':'Bearer '+str(self.token)}
        response = requests.get(url,headers=header)
        self.assertEqual(200, response.status_code)
        data = response.json()
        self.assertEqual(data["count"],1)









