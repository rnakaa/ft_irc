## ソケットfdについて
```
C++（またはC言語）でソケットプログラミングを行う際、socket関数を使用してソケットを作成します。socket関数の返り値は、ソケット記述子（またはソケットファイルディスクリプタ、socket_fd）です。このsocket_fdは、作成されたソケットのユニークな識別子であり、そのソケットへの操作（データの送受信、ソケットの設定変更、ソケットのクローズなど）を行う際に使用します。

socket_fdは、サーバーとクライアントで同じ番号になることはありません。サーバーとクライアントは異なるプロセス（または異なるマシン上のプロセス）で実行されており、それぞれが独立してsocket関数を呼び出してソケットを作成します。各プロセス（またはマシン）は独自のファイルディスクリプタテーブルを持っており、socket_fdはそのテーブル内でユニークですが、異なるプロセス間でのユニーク性は保証されません。

つまり、サーバーがsocket関数を呼び出してソケットを作成したときに得られるsocket_fdと、クライアントがsocket関数を呼び出してソケットを作成したときに得られるsocket_fdは、それぞれ異なる値になります。これらのsocket_fdは、それぞれのプロセスが自身のソケットを操作するために使用します。

サマリーとして、socket_fdの役割は以下の通りです：

    ソケットの識別：socket_fdは作成されたソケットを一意に識別します。
    ソケット操作の実行：送受信などのソケット操作を行う際に、どのソケットに対して操作を行うかを指定するためにsocket_fdを使用します。
    リソースの管理：プログラムが終了するときや、もはやソケットが必要なくなったときに、close関数を使用してsocket_fdを指定し、ソケットを閉じることでリソースを解放します。

異なるプロセス間で通信を行う際には、サーバーがbind、listen、accept関数を使用してクライアントからの接続を待ち受け、クライアントがconnect関数を使用してサーバーに接続するという手順を踏みます。このプロセスでは、各プロセスのsocket_fdは独立しており、互いに直接関連することはありません。
```

## サーバーが複数のクライアントからのの接続

```
サーバーが複数のクライアントからの接続を管理するためには、通常、マルチスレッドまたは非同期I/Oを利用した設計が必要です。各クライアント接続に対して個別のスレッドを割り当てるか、非ブロッキングI/Oを使用して複数の接続を効率的に処理できるようにすることが一般的です。これらのアプローチにより、サーバーは複数のクライアントと同時に対話し、各クライアントからのデータを独立して処理することができます。
```

## ノンブロッキングIOについて

```
ノンブロッキング接続とpoll関数について解説します。これらは、ネットワークプログラミングにおいて非常に重要な概念であり、アプリケーションがI/O操作でブロックされることなく、複数のソケットを効率的に監視・管理することを可能にします。

ノンブロッキング接続

ノンブロッキングI/Oは、I/O操作（例えば、ソケットの読み書きや接続の確立）が即時に完了することを要求せず、操作が完了するまでプログラムの実行を停止させない方法です。ノンブロッキングモードでソケットを設定すると、そのソケット上でのI/O操作は、操作が直ちに完了できない場合でもエラーを返し、プログラムの実行を続行させます。これにより、アプリケーションは他のタスクを続けることができ、特定の操作の完了を非同期的に待つことができます。

poll関数

poll関数は、一つ以上のファイルディスクリプタ（ソケット含む）の状態変化を監視するために使用されます。これにより、アプリケーションは複数のI/O操作を同時に監視し、いずれかが実行可能になった（例えば、読み取り可能なデータがソケットに到着した、またはソケットが書き込みの準備ができた）場合にのみアクションを起こすことができます。poll関数は、指定したファイルディスクリプタの集合に対して、指定された時間（タイムアウト）の間だけ待機し、状態変化があったファイルディスクリプタについての情報を返します。
ノンブロッキングの設定はクライアントとサーバーの両方に必要か？

ノンブロッキングの設定は、その特定のニーズに基づいてクライアントとサーバーのどちらにも適用できます。クライアントがサーバーに接続し、応答を待つ間に他の操作を続行したい場合、クライアント側でノンブロッキングI/Oを使用することが有効です。一方、サーバーが同時に多数のクライアント接続を受け入れて処理する場合、サーバー側でノンブロッキングI/Oを設定することで、特定のクライアントのI/O操作にブロックされることなく、効率的にリソースを管理し、スケーラビリティを向上させることができます。

ノンブロッキングI/Oの利用は、アプリケーションの設計と要件に依存します。一般的には、高いパフォーマンスと効率を必要とするサーバーアプリケーションや、UIの応答性を保ちながらネットワーク操作を行うクライアントアプリケーションでの利用が推奨されます。ノンブロッキングI/Oを適切に管理するためには、select、poll、またはepollのようなI/O多重化メカニズムを使用して、複数のソケットの状態を効率的に監視する必要があります。
```

- ノンブロッキングI/O
    - I/Oの処理中に別の処理を行えるようにすること
        - 通常であればプログラムはユーザがI/Oの処理を終えるまで待機する必要がある
        - しかし、ノンブロッキング状態にすることでI/Oの処理を待たずに関数からの処理が返ってくるので、他の処理を行える。処理をブロック状態にさせないこと=ノンブロッキング
    - 非同期I/Oとの違い
        - 非同期I/OとノンブロッキングI/Oは違う
        - 非同期処理は処理が完了すると通知が返ってくるが、ノンブロッキングは処理が完了した際の通知はなく、処理中の場合はエラー通知が返ってくる
        - プロセスがブロック状態にならないのは両者とも同じではあるが、非同期はI/Oの処理が完了したら通知をして、ノンブロッキングはI/O処理が終わっていないことをエラーで通知する部分が違う
    - 具体例
        - 例えば、c言語において、標準入力(ファイルディスクリプタ)を引数にread関数を実行したら、ユーザが入力を完了するまでread関数の部分で処理が止まる(ブロックする)。しかし、標準入力をノンブロッキングにした状態でread関数を実行すると、ユーザが入力を完了するまではread関数はエラーの値を返り値として返し、処理が止まらなくなる(ノンブロッキングとなる)。なのでread関数の後に特定のエラーの値であればread関数の処理を継続するようエラー処理をする必要がある。
- 複数のクライアントからの接続を対応(TLPI 63.1 概要 を参照)
    - ソケット通信において、通常の実装だとサーバーに対してクライアントが接続できるのは1つまでで、1:Nは対応できていない
        - 問題点のある解決策
            - サーバーが保持している全てのクライアントのソケットファイルディスクリプタをノンブロッキング状態にして、定期的に入力可能か確認する
                - 問題点として、全てのファイルディスクリプタは順番に確認する必要があり、1つ1つのファイルディスクリプタの確認時間が長いと別のファイルディスクリプタのI/Oイベントに反応するのが遅くなり、逆に確認時間が短いとCPU時間の浪費となってしまう
            - クライアントごとにプロセスかスレッドを新たに作成する
                - 問題点として、クライアントが増えれば増えるほどリソース消費が激しくなり、システムにとって負荷が高い
        - 最適なモデル
            - **poll関数かselect関数を使ったI/O多重化(今回はこれを使用する)**
                -  poll関数やselect関数を使って、1つプロセスで複数のファイルディスクリプタを同時に監視し、I/Oが可能になったファイルディスクリプタを特定する
            - シグナルドリブンI/O
                - I/Oイベントが発生したらカーネルからプロセスへシグナルを送信する
            - Linux固有のepoll関数を使用する
                - poll関数やselect関数の上位互換、クライアントが増えた際の処理速度が速い
- poll() (TLPI 63.2.2 poll() システムコール を参照)
    ```c
    int poll(struct pollfd fds[], nfds_t nfds, int timeout);

    struct pollfd {
        int   fd; /* ファイルディスクリプタ */
        short events; /* 監視イベントビットマスク */
        short revents; /* 発生イベントビットマスク */
    }
    ```
    - fdsには監視対象のファイルディスクリプタをpollfd構造体の配列として指定し、nfdsにはその要素数を指定し、timeoutにはpoll()がブロックする時間の上限を指定
    - struct pollfd
        - eventsにはどの条件が発生を監視するのかを指定できる。reventsは実際にどの条件が発生したのかがpoll関数の実行後に格納される
            - ソケットに対するフラグの種類は、読み取り可能が`POLLIN`、書き込み可能が`POLLOUT`、リスニングソケットが接続要求を確立したのが`POLLIN`となっている
    - timeout
        - ブロック動作を指定できる
        - -1は、対応するeventsに一致するI/Oイベントが発生するまでブロックする
        - 0は、ブロックしない
        - 生の値は、指定した値のm秒分までブロックする
    - 返り値
        - -1はエラー、0はタイムアウト、1以上はI/O可能になったファイルディスクリプタ数
- recv, sendのフラグ(TLPI 61.3 ソケット専用 I/O システムコール:recv() と send() を参照)
    - recv()とsend()は関数の引数を特定のフラグにするとノンブロッキング状態にできる
    ```c++
        ssize_t recv(int sockfd, void *buffer, size_t length, int flags);
        ssize_t send(int sockfd, const void *buffer, size_t length, int flags);
    ```
    - MSG_DONTWAIT
        - ノンブロッキングモードで recv() を実行する。読み取るデータが存在しない場合にブ ロックせず、その場で EAGAIN のエラーとする。ソケットに対し fcntl() を実行し、ノンブロッキングモードを設定しても同様の動作となるが(O_NONBLOCK)、MSG_DONTWAIT では recv() を実行するたびに動作を選択できる柔軟性がある。