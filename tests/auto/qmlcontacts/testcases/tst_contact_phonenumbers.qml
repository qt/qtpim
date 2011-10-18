import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

TestCase {
    name: "ContactsListPropertiesTests"

    TestCase {
        name: "ContactsListPropertiesTests::NoPhoneNumber"

        Contact {
            id: contactWithoutPhoneNumber
        }

        function test_phoneNumberOfContactWithoutPhoneNumberIsEmpty() {
            verifyIsUndefined(contactWithoutPhoneNumber.phoneNumber);
        }

        function test_phoneNumbersCountOfContactWithoutPhoneNumberIsZero() {
            compare(contactWithoutPhoneNumber.phoneNumbers.length, 0);
        }
    }

    TestCase {
        name: "ContactsListPropertiesTests::OnePhoneNumber"

        Contact {
            id: contactWithOnePhoneNumber

            PhoneNumber {
                number: "1111111111"
                subTypes:[PhoneNumber.Mobile]
            }
        }

        function test_accessPhoneNumberThroughPhoneNumber() {
            compare(contactWithOnePhoneNumber.phoneNumber.number, "1111111111");
        }

        function test_phoneNumberCount() {
            compare(contactWithOnePhoneNumber.phoneNumbers.length, 1);
        }

        function test_accessPhoneNumberThroughPhoneNumbers() {
            compare(contactWithOnePhoneNumber.phoneNumbers[0].number, "1111111111");
        }

        function test_accessPhoneNumberThroughDetails() {
            compare(contactWithOnePhoneNumber.details(
                        ContactDetail.PhoneNumber)[0].number, "1111111111");
        }
    }

    TestCase {
        name: "ContactsListPropertiesTests::MultiplePhoneNumbers"

        Contact {
            id: contactWithPhoneNumbers

            PhoneNumber {
                number: "1111111111"
                subTypes:[PhoneNumber.Mobile]
            }

            PhoneNumber {
                number: "2222222222"
                subTypes:[PhoneNumber.Fax]
            }
        }

        function test_accessPhoneNumberThroughPhoneNumber() {
            compare(contactWithPhoneNumbers.phoneNumber.number, "1111111111");
        }

        function test_phoneNumberCount() {
            compare(contactWithPhoneNumbers.phoneNumbers.length, 2);
        }

        function test_accessPhoneNumberThroughPhoneNumbers() {
            compare(contactWithPhoneNumbers.phoneNumbers[0].number, "1111111111");
            compare(contactWithPhoneNumbers.phoneNumbers[1].number, "2222222222");
        }

        function test_accessPhoneNumberThroughDetails() {
            compare(contactWithPhoneNumbers.details(
                        ContactDetail.PhoneNumber)[0].number, "1111111111");
        }
    }

    TestCase {
        name: "ContactsListPropertiesTests::DynamicallyAdded::MultiplePhoneNumbers"
        Contact {
            id: contact1
        }

        PhoneNumber {
            id: phoneNumber1
            number: "1111111111"
            subTypes:[PhoneNumber.Mobile]
        }

        PhoneNumber {
            id: phoneNumber2
            number: "2222222222"
            subTypes:[PhoneNumber.Fax]
        }

        function init() {
            contact1.clearDetails();
            contact1.addDetail(phoneNumber1);
            contact1.addDetail(phoneNumber2);
        }

        function test_accessPhoneNumberThroughPhoneNumber() {
            compare(contact1.phoneNumber.number, "1111111111");
        }

        function test_phoneNumberCount() {
            verify(contact1.phoneNumbers);
            compare(contact1.phoneNumbers.length, 2);
        }

        function test_accessPhoneNumberThroughPhoneNumbers() {
            compare(contact1.phoneNumbers[0].number, "1111111111");
            compare(contact1.phoneNumbers[1].number, "2222222222");
        }

        function test_accessPhoneNumberThroughDetails() {
            compare(contact1.details(
                        ContactDetail.PhoneNumber)[0].number, "1111111111");
        }

        function cleanup () {
            contact1.clearDetails();
        }
    }

    function verifyIsUndefined(object) {
        verify(!object, "Object " + object + " is undefined");
    }
}

