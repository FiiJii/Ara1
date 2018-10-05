from django.test import TestCase
from rest_framework.test import APILiveServerTestCase, APITestCase
import requests
import json
from api_trading.test import TradingBaseTestCase
from django.contrib.auth.models import User


class UserLoginTokenTestCase(TradingBaseTestCase):
    
    def setUp(self):
       self.setupUser();

    def test_authentication_without_password(self):
        url = self.get_url_server()+"api/auth/token/"
        response = requests.post(url, {"username":"snowman"})
        self.assertEqual(400, response.status_code)
    
    def test_authentication_wrong_password(self):
        url = self.get_url_server()+"api/auth/token/"
        data = {"username":self.username,"password":"I_know"}
        response = requests.post(url, json=data)
        self.assertEqual(400, response.status_code)
    
    def test_authentication_with_valid_data(self):
        url = self.get_url_server()+"api/auth/token/"
        data = {"username":self.username,"password": self.password}
        response = requests.post(url,json=data)
        self.assertEqual(200, response.status_code)
        self.assertTrue("access" in json.loads(response.content))


class UserRegisterSuccessTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupSuperUser();
        self.setupToken();

    def test_register_user_successfully(self):
        url = self.get_url_server()+"api/account/users/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "username":"lordsnow",
            "password":"123456789",
            "first_name":"John",
            "last_name":"Snow",
            "email":"john@snow.com",
            "is_staff": True,
            "is_superuser": False,
            "is_active": True
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(201, response.status_code)

class UserRegisterWithErrorsTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupSuperUser();
        self.setupToken();
        
        url = self.get_url_server()+"api/account/users/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "username":"lordsnow",
            "password":"123456789",
            "first_name":"John",
            "last_name":"Snow",
            "email":"john@snow.com",
            "is_staff": True,
            "is_superuser": False,
            "is_active": True
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(201, response.status_code)


    def test_register_user_with_username_already_registered(self):
        url = self.get_url_server()+"api/account/users/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "username":"lordsnow",
            "password":"987654321",
            "first_name":"Robb",
            "last_name":"Stark",
            "email":"robb@stark.com",
            "is_staff": True,
            "is_superuser": False,
            "is_active": True
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(400, response.status_code)
        self.assertEqual(json.loads(response.content)["username"], ["This field must be unique."])

    def test_register_user_without_username(self):
        url = self.get_url_server()+"api/account/users/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "password":"987654321",
            "first_name":"Robb",
            "last_name":"Stark",
            "email":"robb@stark.com",
            "is_staff": True,
            "is_superuser": False,
            "is_active": True
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(400, response.status_code)
        self.assertEqual(json.loads(response.content)["username"], ["This field is required."])


    def test_register_user_with_email_already_registered(self):
        url = self.get_url_server()+"api/account/users/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "username":"lordsnow",
            "password":"987654321",
            "first_name":"Robb",
            "last_name":"Stark",
            "email":"john@snow.com",
            "is_staff": True,
            "is_superuser": False,
            "is_active": True
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(400, response.status_code)
        self.assertEqual(json.loads(response.content)["email"], ["This field must be unique."])
    
    def test_register_user_without_email(self):
        url = self.get_url_server()+"api/account/users/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "username":"lordsnow",
            "password":"987654321",
            "first_name":"Robb",
            "last_name":"Stark",
            "is_staff": True,
            "is_superuser": False,
            "is_active": True
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(400, response.status_code)
        self.assertEqual(json.loads(response.content)["email"], ["This field is required."])
    
    def test_register_user_without_password(self):
        url = self.get_url_server()+"api/account/users/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "username":"ladyofwinterfel",
            "first_name":"Sansa",
            "last_name":"Stark",
            "email":"sansa@stark.com",
            "is_staff": True,
            "is_superuser": False,
            "is_active": True
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(400, response.status_code)
        self.assertEqual(json.loads(response.content)["password"], ["This field is required."])

    def test_register_user_with_password_less_than_the_allowed_characters(self):
        url = self.get_url_server()+"api/account/users/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "username":"Matareyes",
            "password":"caterly",
            "first_name":"Jaime",
            "last_name":"Lannister",
            "email":"jaime@lannister.com",
            "is_staff": True,
            "is_superuser": False,
            "is_active": True
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(400, response.status_code)
        self.assertEqual(json.loads(response.content)["password"], ["Ensure this field has at least 8 characters."])


class ChangePasswordTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupSuperUser();
        self.setupToken();

        url = self.get_url_server()+"api/account/users/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "username":"enano",
            "password":"123456789",
            "first_name":"Tyrion",
            "last_name":"Lannister",
            "email":"tyrion@lannisport.com",
            "is_staff": True,
            "is_superuser": False,
            "is_active": True
            }
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(201, response.status_code)
        self.user_id=response.json()["id"]
        print(self.user_id)

    def test_change_password_sucessfully(self):
        url = self.get_url_server()+"api/account/users/"+str(self.user_id)+"/change_password/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            	"new_password":"987654321"
            }
        
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(200, response.status_code)
        self.assertEqual(json.loads(response.content)["status"], "password changed successfully")
    
    def test_change_password_without_new_password(self):
        url = self.get_url_server()+"api/account/users/"+str(self.user_id)+"/change_password/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            	"new_password":""
            }
        
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(400, response.status_code)
        self.assertEqual(json.loads(response.content)["new_password"], ["This field may not be blank."])

    def test_change_password_with_new_password_wrong(self):
        url = self.get_url_server()+"api/account/users/"+str(self.user_id)+"/change_password/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            	"new_password":"1234"
            }
        
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(400, response.status_code)
        self.assertEqual(json.loads(response.content)["new_password"], ["Ensure this field has at least 8 characters."])

        

    

    
