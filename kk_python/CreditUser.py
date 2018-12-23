import ctypes

lib2 = ctypes.cdll.LoadLibrary('./libCreditUser.so')

class CCreditUser(object):
    def __init__(self):
        lib2.CCreditUser_new.argtypes = []
        lib2.CCreditUser_new.restype = ctypes.c_void_p
        lib2.CCreditUser_setRate.argtypes = [ctypes.c_void_p,ctypes.c_double]
        lib2.CCreditUser_setRate.restype = ctypes.c_void_p
        lib2.CCreditUser_setAnuityLimit.argtypes = [ctypes.c_void_p,ctypes.c_double]
        lib2.CCreditUser_setAnuityLimit.restype = ctypes.c_void_p
        lib2.CCreditUser_AddName.argtypes = [ctypes.c_void_p,ctypes.c_char_p]
        lib2.CCreditUser_AddName.restype = ctypes.c_void_p
        lib2.CCreditUser_setTotalAmmount.argtypes = [ctypes.c_void_p,ctypes.c_double]
        lib2.CCreditUser_setTotalAmmount.restype = ctypes.c_void_p
        lib2.CCreditUser_calculateTotalAnuityOnRate.argtypes = [ctypes.c_void_p]
        lib2.CCreditUser_calculateTotalAnuityOnRate.restype = ctypes.c_void_p
        lib2.CCreditUser_calculateRateOnAmmount.argtypes = [ctypes.c_void_p]
        lib2.CCreditUser_calculateRateOnAmmount.restype = ctypes.c_void_p
        lib2.CCreditUser_getAnuityOnYear.argtypes = [ctypes.c_void_p,ctypes.c_int]
        lib2.CCreditUser_getAnuityOnYear.restype = ctypes.c_double
        lib2.CCreditUser_getAmmountOnYear.argtypes = [ctypes.c_void_p,ctypes.c_int]
        lib2.CCreditUser_getAmmountOnYear.restype = ctypes.c_double
        self.obj = lib2.CCreditUser_new()

    def setRate(self, rate):
        lib2.CCreditUser_setRate(self.obj, rate)

    def setAnuityLimit(self, anuity):
        lib2.CCreditUser_setAnuityLimit(self.obj, anuity)

    def AddName(self, name):
        lib2.CCreditUser_AddName(self.obj, name)

    def setTotalAmmount(self, ammount):
        lib2.CCreditUser_setTotalAmmount(self.obj, ammount)

    def calculateTotalAnuityOnRate(self):
        lib2.CCreditUser_calculateTotalAnuityOnRate(self.obj)

    def calculateRateOnAmmount(self):
        lib2.CCreditUser_calculateRateOnAmmount(self.obj)

    def getAnuityOnYear(self, year):
        return lib2.CCreditUser_getAnuityOnYear(self.obj, year)

    def getAmmountOnYear(self, year):
        return lib2.CCreditUser_getAmmountOnYear(self.obj, year)

print "Loaded CreditUser module"
#us = CCreditUser()
#us.setRate(3.4)

#us.setAnuityLimit(500)
#us.AddName("Bogdan")
#us.setTotalAmmount(30000)
#us.calculateTotalAnuityOnRate()
#us.calculateRateOnAmmount()
#print "anuitet za trinaestu godinu"
#print us.getAnuityOnYear(12)
