import 'package:firebase_database/firebase_database.dart';
import 'package:firebase_database/ui/firebase_animated_list.dart';
import 'package:flutter/material.dart';


class FetchData extends StatefulWidget {
  const FetchData({Key? key}) : super(key: key);

  @override
  State<FetchData> createState() => _FetchDataState();
}

class _FetchDataState extends State<FetchData> {
  late Query dbRef;
  DatabaseReference reference = FirebaseDatabase.instance.reference()
      .child('gaslogs').child('tly0iR42HnNrpYBFBTJVSmkkZSC2').child('readings');

  Widget listItem({required Map data, required String key}) {
    return Container(
      margin: const EdgeInsets.all(10),
      padding: const EdgeInsets.all(10),
      height: 110,
      color:  Color(0xffeabd1e),
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text(
            'amount: ${data['amount']}',
            style: TextStyle(fontSize: 16, fontWeight: FontWeight.w400),
          ),
          const SizedBox(
            height: 5,
          ),
          Text(
            'timestamp: ${data['timestamp']}',
            style: TextStyle(fontSize: 16, fontWeight: FontWeight.w400),
          ),
        ],
      ),
    );
  }

  @override
  void initState() {
    super.initState();
    dbRef = reference;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Station logs',
          style: TextStyle(
            fontSize: 26,
            fontWeight: FontWeight.w500,
          ),),
        centerTitle: true,
        backgroundColor: Color(0xffdc143c),
      ),
      body: Container(
        height: double.infinity,
        child: FirebaseAnimatedList(
          query: dbRef,
          itemBuilder: (BuildContext context, DataSnapshot snapshot,
              Animation<double> animation, int index) {
            Map data = snapshot.value as Map;
            String key = snapshot.key!;

            return listItem(data: data, key: key);
          },
        ),
      ),
    );
  }
}