import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';

class WriteData extends StatefulWidget {
  const WriteData({Key? key}) : super(key: key);

  @override
  State<WriteData> createState() => _WriteDataState();
}

class _WriteDataState extends State<WriteData> {
  DatabaseReference reference = FirebaseDatabase.instance.reference()
      .child('gaslogs').child('tly0iR42HnNrpYBFBTJVSmkkZSC2').child('readings');

  TextEditingController amountController = TextEditingController(); // Controller for the text input

  void writeData() {
    String enteredAmount = amountController.text;
    Map<String, dynamic> newData = {
      'amount': enteredAmount + " Liters",
      'timestamp': DateTime.now().toString(),
    };
    reference.push().set(newData);

    amountController.clear();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(
          'Enter Amount',
          style: TextStyle(
            fontSize: 26,
            fontWeight: FontWeight.w500,
          ),
        ),
        centerTitle: true,
        backgroundColor: Color(0xffdc143c),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            TextField(
              controller: amountController,
              keyboardType: TextInputType.number,
              // Input type is number
              decoration: InputDecoration(
                labelText: 'Enter Amount',
                border: OutlineInputBorder(),
              ),
            ),
            SizedBox(height: 20),
            ElevatedButton(
              onPressed: writeData,
              child: Text('Enter'),
            ),
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    amountController.dispose(); // Dispose the controller to avoid memory leaks
    super.dispose();
  }
}
