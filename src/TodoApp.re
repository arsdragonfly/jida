type action =
  | AddItem(string)
  | ToggleItem(int);

type item = {
  id: int,
  title: string,
  completed: bool,
};

type state = {
  id: int, // id for a new item
  items: list(item),
};

let str = React.string;

module TodoItem = {
  [@react.component]
  let make = (~item, ~onToggle) => {
    <div className="item" onClick={_evt => onToggle()}>
      <input type_="checkbox" checked={item.completed} />
      {str(item.title)}
    </div>;
  };
};

let valueFromEvent = (evt): string => evt->ReactEvent.Form.target##value;

module Input = {
  type state = string;
  [@react.component]
  let make = (~onSubmit) => {
    let (text, setText) =
      React.useReducer((oldText, newText) => newText, "");
    <input
      value=text
      type_="text"
      placeholder="Write something to do"
      onChange={evt => setText(valueFromEvent(evt))}
      onKeyDown={evt =>
        if (ReactEvent.Keyboard.key(evt) == "Enter") {
          onSubmit(text);
          setText("");
        }
      }
    />;
  };
};

[@react.component]
let make = (~title) => {
  let (state, dispatch) =
    React.useReducer(
      (state: state, action: action) => {
        let newItem = (text) => {
          id: state.id,
          title: text,
          completed: false,
        };
        switch (action) {
        | AddItem(text) => {id: state.id + 1, items: [newItem(text), ...state.items]}
        | ToggleItem(id) =>
          let items =
            List.map(
              (item: item) =>
                item.id === id ? {...item, completed: !item.completed} : item,
              state.items,
            );
          {id: state.id, items};
        };
      },
      {
        id: 1,
        items: [{id: 0, title: "Write some things to do", completed: false}],
      },
    );
  let numItems = List.length(state.items);
  <div className="app">
    <div className="title">
      {str("What to do")}
      <Input onSubmit=((text) => dispatch(AddItem(text))) />
    </div>
    <div className="items">
      {React.array(
         Array.of_list(
           List.map(
             (item: item) =>
               <TodoItem
                 item
                 onToggle={() => dispatch(ToggleItem(item.id))}
               />,
             state.items,
           ),
         ),
       )}
    </div>
    <div className="footer">
      {str(string_of_int(numItems) ++ " items")}
    </div>
  </div>;
};